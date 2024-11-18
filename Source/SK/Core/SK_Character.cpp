// 

// Base:
#include "SK_Character.h"

// UE:
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Interaction:
#include "SK_PlayerController.h"
#include "SK/Core/SK_GameMode.h"
#include "SK/ChessBoard/Square.h"
#include "SK/ChessMans/ChessMan.h"
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
    SubscribeToDelegates();
}

void ASK_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    MovementForTick(DeltaTime);
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
        if (CurrentChessOperator)
        {
            CurrentChessOperator->StopTimer_MovesSequence();
        }

        Destroy();

        Cast<ASK_GameMode>(GetWorld()->GetAuthGameMode())->SetLosingGame();
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
    // Вычисление дельты для текущего кадра на основе информации о скорости
    AddControllerYawInput(Rate * BaseTurnRate);
}

void ASK_Character::LookUpAtRate(float Rate)
{
    // Вычисление дельты для текущего кадра на основе информации о скорости
    AddControllerPitchInput(-Rate * BaseLookUpRate);
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
        // Освободить предыдущую клетку и занять новую
        if (CurrentSquare)
            CurrentSquare->OccupySquare(EWarringPartiesType::NONE);
        ToSquare->OccupySquare(EWarringPartiesType::White);

        CurrentSquare = ToSquare;

        NewLocation = ToSquare->GetActorLocation();
        NewLocation.Z = GetActorLocation().Z;

        bIsMovingToNewLocation = true;
        bIsMoveAllowed = false;

        if (CurrentChessOperator)
        {
            CurrentChessOperator->StopTimer_MovesSequence();
        }

        EnableMouse(false);
    }
    else if (!ToSquare)
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': ToSquare is NOT"),
            *GetNameSafe(this));
    }
}

FIndex2D ASK_Character::GetCurrentPosition()
{
    return CurrentSquare->GetData().PositionNumber;
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

            if (CurrentChessOperator)
            {
                CurrentChessOperator->OnPlayersMove.Broadcast(false);
            }
        }
        else
        {
            // Плавная интерполяция перемещения
            SetActorLocation(FMath::VInterpTo(lCurrentLocation, NewLocation, lDeltaTime, MovementSpeed));
        }
    }
}

void ASK_Character::EnableMouse(bool bEnabled)
{
    if (CurrentPlayerController)
    {
        CurrentPlayerController->bEnableClickEvents = bEnabled;
        CurrentPlayerController->bEnableTouchEvents = bEnabled;
    }
}
//--------------------------------------------------------------------------------------



/* ---   Player Moves Sequence   --- */

void ASK_Character::PlayerMovesSequence(bool bIsPlayersMove)
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

void ASK_Character::SubscribeToDelegates()
{
    TArray<AActor*> lResultActors;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChessOperator::StaticClass(), lResultActors);

    if (lResultActors.IsValidIndex(0))
    {
        CurrentChessOperator = Cast<AChessOperator>(lResultActors[0]);
    }

    if (CurrentChessOperator)
    {
        CurrentChessOperator->OnPlayersMove.AddDynamic(this, &ASK_Character::PlayerMovesSequence);
    }
}
//--------------------------------------------------------------------------------------
