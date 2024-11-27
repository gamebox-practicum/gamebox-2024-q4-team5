//

// Base:
#include "SK_Character.h"

// UE:
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Interaction:
#include "SK_PlayerController.h"
#include "SK/Core/SK_GameMode.h"
#include "SK/ChessBoard/Square.h"
#include "SK/ChessMans/ChessMan.h"
#include "SK/ChessMans/ChessManGenerator.h"
#include "SK/ChessOperators/ChessOperator.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

ASK_Character::ASK_Character()
{
    // Установка вызова функции Tick() в каждом кадре.
    PrimaryActorTick.bCanEverTick = true; // Warning: Принудительно!


    /* ---   Components   --- */

    // Корневой компонент
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Капсула коллизии
    GetCapsuleComponent()->SetupAttachment(RootComponent);

    // Меш с манекеном. Необходим для визуализации движения камеры
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    // Камера от первого лица
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
    FirstPersonCameraComponent->SetupAttachment(GetMesh(), FName(TEXT("head")));
    FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 10.f, 0.f));
    FirstPersonCameraComponent->SetRelativeRotation(FRotator(0.f, 90.f, -90.f));
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    // Меш Шахматной Фигуры со скелетом
    ChessmanSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Chessman Skeletal Mesh"));
    ChessmanSkeletalMesh->SetupAttachment(GetCapsuleComponent());

    // Статичный Меш Шахматной Фигуры
    ChessmanStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chessman Static Mesh"));
    ChessmanStaticMesh->SetupAttachment(GetCapsuleComponent());
    ChessmanStaticMesh->SetGenerateOverlapEvents(false);
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void ASK_Character::BeginPlay()
{
    Super::BeginPlay();

    Cleaning();

    Initialize();
}

void ASK_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    MovementForTick(DeltaTime);
    RotateToChessManForTick(DeltaTime);
}

void ASK_Character::PossessedBy(AController* NewController)
{
    CurrentPlayerController = Cast<ASK_PlayerController>(NewController);
}

void ASK_Character::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (Cast<AChessMan>(OtherActor))
    {
        CharacterDeath();
    }
}

void ASK_Character::Initialize()
{
    SubscribeToDelegates();

    // Подтянуть игрока
    if (!CurrentPlayerController
        && AutoPossessPlayer != EAutoReceiveInput::Disabled)
    {
        const int32 PlayerIndex = int32(AutoPossessPlayer.GetValue()) - 1;

        APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);
        if (PC)
        {
            PC->Possess(this);
        }
    }
}

void ASK_Character::Cleaning()
{
    if (ChessmanSkeletalMesh && !(ChessmanSkeletalMesh->SkeletalMesh))
    {
        ChessmanSkeletalMesh->DestroyComponent();
    }

    if (ChessmanStaticMesh && !(ChessmanStaticMesh->GetStaticMesh()))
    {
        ChessmanStaticMesh->DestroyComponent();
    }
}
//--------------------------------------------------------------------------------------



/* ---   Input Action   --- */

void ASK_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Перемещение:
    PlayerInputComponent->BindAxis("MoveForward", this, &ASK_Character::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASK_Character::MoveRight);

    // Поворот мышью:
    PlayerInputComponent->BindAxis("TurnRate", this, &ASK_Character::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUpRate", this, &ASK_Character::LookUpAtRate);
}

void ASK_Character::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        // add movement in that direction
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void ASK_Character::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        // add movement in that direction
        AddMovementInput(GetActorRightVector(), Value);
    }
}

void ASK_Character::TurnAtRate(float Rate)
{
    if (!bPlayerControlLock)
    {
        // Вычисление дельты для текущего кадра на основе информации о скорости
        AddControllerYawInput(Rate * BaseTurnRate);
    }
}

void ASK_Character::LookUpAtRate(float Rate)
{
    if (!bPlayerControlLock)
    {
        // Вычисление дельты для текущего кадра на основе информации о скорости
        AddControllerPitchInput(-Rate * BaseLookUpRate);
    }
}
//--------------------------------------------------------------------------------------



/* ---   Movement   --- */

void ASK_Character::MoveToSquare(ASquare* ToSquare)
{
    if (bIsMoveAllowed
        && ToSquare
        && CurrentSquare != ToSquare
        && !bIsMovingToNewLocation)
    {
        if (CurrentOperator)
        {
            CurrentOperator->StopTimer_MovesSequence();
        }

        // Освободить предыдущую клетку и занять новую
        if (CurrentSquare)
            CurrentSquare->OccupySquare(EWarringPartiesType::NONE);
        ToSquare->OccupySquare(EWarringPartiesType::White);

        CurrentSquare = ToSquare;
        CurrentPosition = CurrentSquare->GetData().PositionNumber;

        NewLocation = ToSquare->GetActorLocation();
        NewLocation.Z = GetActorLocation().Z;

        bIsMovingToNewLocation = true;
        bIsMoveAllowed = false;

        EnableMouse(false);
    }
    else if (!ToSquare)
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': ToSquare is NOT"),
            *GetNameSafe(this));
    }

    EventOnMove();
}

FIndex2D ASK_Character::GetCurrentPosition() const
{
    return CurrentPosition;
}

void ASK_Character::MovementForTick(const float& lDeltaTime)
{
    // Контроль перемещения
    if (bIsMovingToNewLocation)
    {
        FVector lCurrentLocation = GetActorLocation();

        // Контроль близости к новой локации
        if ((lCurrentLocation - NewLocation).Size() < MaxDeviation)
        {
            SetActorLocation(NewLocation);
            bIsMovingToNewLocation = false;

            if (CurrentOperator)
            {
                CurrentOperator->OnPlayersMove.Broadcast(false);
            }
        }
        else
        {
            // Плавная интерполяция перемещения
            SetActorLocation(FMath::VInterpTo(lCurrentLocation, NewLocation, lDeltaTime, MovementSpeed));
        }
    }
}

void ASK_Character::EnableMouse(const bool& bEnabled)
{
    if (CurrentPlayerController)
    {
        CurrentPlayerController->EnableMouseEvents(bEnabled);
    }
}
//--------------------------------------------------------------------------------------



/* ---   Player Moves Sequence   --- */

void ASK_Character::PlayerMovesSequence(const bool& bIsPlayersMove)
{
    if (bIsPlayersMove)
    {
        bIsMoveAllowed = bIsPlayersMove;

        EnableMouse(true);
    }
    else
    {
        EnableMouse(false);
    }
}

void ASK_Character::SetPointerToOperator(AChessOperator* iCurrentOperator)
{
    CurrentOperator = iCurrentOperator;
}

void ASK_Character::SetCurrentChessManGenerator(AChessManGenerator* iGenerator)
{
    CurrentChessManGenerator = iGenerator;
}

void ASK_Character::SubscribeToDelegates()
{
    if (CurrentOperator)
    {
        CurrentOperator->OnPlayersMove.AddUObject(this, &ASK_Character::PlayerMovesSequence);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': CurrentOperator is NOT"),
            *GetNameSafe(this));
    }
}
//--------------------------------------------------------------------------------------



/* ---   Rotation   --- */

void ASK_Character::RotateToChessMan(AChessMan* bIsPlayersMove)
{
    if (bIsPlayersMove)
    {
        ChessManForRotation = bIsPlayersMove;

        bPlayerControlLock = true;
    }
}

void ASK_Character::RotateToChessManForTick(const float& lDeltaTime)
{
    if (bPlayerControlLock)
    {
        FRotator lViewRotation = CurrentPlayerController->GetControlRotation();
        FRotator lEndRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ChessManForRotation->GetActorLocation());

        FRotator lResult = UKismetMathLibrary::RInterpTo(lViewRotation, lEndRotation, lDeltaTime, 1.f);

        CurrentPlayerController->SetControlRotation(lResult);
    }
}
//--------------------------------------------------------------------------------------


/* ---   Death   --- */

void ASK_Character::CharacterDeath()
{
    if (CurrentOperator)
    {
        CurrentOperator->StopTimer_MovesSequence();
    }

    if (CurrentChessManGenerator)
    {
        CurrentChessManGenerator->RemovePlayer(this);
    }

    EventOnDeath();

    Cast<ASK_GameMode>(GetWorld()->GetAuthGameMode())->SetLosingGame();
}
//--------------------------------------------------------------------------------------
