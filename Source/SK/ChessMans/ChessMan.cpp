//

// Base:
#include "ChessMan.h"

// UE:
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

// Interaction:
#include "ChessManGenerator.h"
#include "SK/ChessBoard/Square.h"
#include "SK/ChessBoard/SquareComponent.h"
#include "SK/ChessOperators/ChessOperator.h"
#include "SK/ChessOperators/DealerHand.h"
#include "SK/Core/SK_Character.h"

// Tools:
#include "SK/Tools/MyRandom.h"
#include "SK/Tools/ActorComponents/ActorMovementComponent.h"
#include "SK/Tools/ActorComponents/ActorRotationComponent.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values
AChessMan::AChessMan()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Предварительно



    /* ---   Components   --- */

    // Корневой компонент
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Капсула коллизии
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
    CapsuleComponent->SetupAttachment(RootComponent);
    CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

    // Меш Шахматной Фигуры со скелетом
    ChessmanSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Chessman Skeletal Mesh"));
    ChessmanSkeletalMesh->SetupAttachment(CapsuleComponent);
    ChessmanSkeletalMesh->SetCustomDepthStencilValue(1);
    ChessmanSkeletalMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

    // Статичный Меш Шахматной Фигуры
    //ChessmanStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chessman Static Mesh"));
    //ChessmanStaticMesh->SetupAttachment(RootComponent);
    //ChessmanStaticMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    //ChessmanStaticMesh->SetCustomDepthStencilValue(1);

    // Точка местоположения Места Захвата данной фигуры Рукой Дилера
    CapturePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Capture Point"));
    CapturePoint->SetupAttachment(RootComponent);

    // Компонент перемещения данного Актора
    MovementComponent = CreateDefaultSubobject<UActorMovementComponent>(TEXT("Movement Component"));

    // Компонент вращения данного Актора
    RotationComponent = CreateDefaultSubobject<UActorRotationComponent>(TEXT("Rotation Component"));
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

// Called when the game starts or when spawned
void AChessMan::BeginPlay()
{
    Super::BeginPlay();

    Cleaning();
    SubscribeToDelegates();
    RotateToFirstPlayer();
}

// Called every frame
void AChessMan::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AChessMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChessMan::Destroyed()
{
    if (CurrentSquare)
    {
        CurrentSquare->OccupySquare(EWarringPartiesType::NONE);
    }

    Super::Destroyed();
}

void AChessMan::Cleaning()
{
    if (ChessmanSkeletalMesh && !(ChessmanSkeletalMesh->SkeletalMesh))
    {
        ChessmanSkeletalMesh->DestroyComponent();
        CapsuleComponent->DestroyComponent();
    }

    //if (ChessmanStaticMesh && !(ChessmanStaticMesh->GetStaticMesh()))
    //{
    //    ChessmanStaticMesh->DestroyComponent();
    //}
}

void AChessMan::Initialize()
{
    if (CurrentData.bIsDead)
    {
        ChessManPreDeath();

        OnOriginallyDeath.Broadcast();
    }
    else
    {
        RotationInit();
        SubscribeToDelegates();
        RotateToFirstPlayer();
    }

    /* ---   По завершении создания и инициализации:   --- */

    // Задействовать событие завершении Инициализации
    EventOnInitializeComplete();
}
//--------------------------------------------------------------------------------------



/* ---   Interaction   --- */

void AChessMan::NotifyActorBeginCursorOver()
{
    Super::NotifyActorBeginCursorOver();

    if (GetWorldSettings()->GetPauserPlayerState() != NULL)
    {
        return;
    }

    if (!bIsDead)
    {
        if (ChessmanSkeletalMesh)
            ChessmanSkeletalMesh->SetRenderCustomDepth(true);
        //if (ChessmanStaticMesh)
        //    ChessmanStaticMesh->SetRenderCustomDepth(true);
    }
}

void AChessMan::NotifyActorEndCursorOver()
{
    Super::NotifyActorEndCursorOver();

    if (ChessmanSkeletalMesh)
        ChessmanSkeletalMesh->SetRenderCustomDepth(false);
    //if (ChessmanStaticMesh)
    //    ChessmanStaticMesh->SetRenderCustomDepth(false);
}

void AChessMan::NotifyActorOnClicked(FKey ButtonReleased)
{
    Super::NotifyActorOnClicked(ButtonReleased);

    if (ButtonReleased == EKeys::LeftMouseButton)
    {
        ChessManDeath();
    }
}
//--------------------------------------------------------------------------------------



/* ---   Type   --- */

int32 AChessMan::GetCurrentSquareType() const
{
    if (CurrentSquare)
    {
        return CurrentSquare->GetMaterialType();
    }
    return -1;
}
//--------------------------------------------------------------------------------------



/* ---   Movement   --- */

void AChessMan::MoveToSquare(ASquare* ToSquare)
{
    if (ToSquare)
    {
        if (ToSquare->GetData().WarringPartiesType == EWarringPartiesType::White)
        {
            RotationComponent->RotateToLocation(CurrentFirstPlayer->GetActorLocation());
            OnEatingChessman.Broadcast();
        }

        bIsMovingToNewLocation = true;

        CheckMovementType(ToSquare);

        SetCurrentSquare(ToSquare);

        // Сохранение данных
        CurrentData.Position = CurrentSquare->GetData().PositionNumber;

        if (CurrentOperator)
        {
            CurrentOperator->bIsPlayersMove = true;
        }

        // Запуск перемещения Руки Дилера
        OnSelected.Broadcast();
        CurrentDealerHand->MovementComponent->OnCompletedMove.AddDynamic(this, &AChessMan::DealerHandMovementEnd);
        CurrentDealerHand->MoveToLocation(CapturePoint->GetComponentLocation());
    }
}

void AChessMan::SetCurrentSquare(ASquare* NewSquare)
{
    // Освободить предыдущую клетку и занять новую
    if (CurrentSquare)
        CurrentSquare->OccupySquare(EWarringPartiesType::NONE);
    NewSquare->OccupySquare(EWarringPartiesType::Black);

    CurrentSquare = NewSquare;
}

void AChessMan::SetPointerToOperator(AChessOperator* iCurrentOperator)
{
    CurrentOperator = iCurrentOperator;
}

void AChessMan::SetCurrentDealerHand(ADealerHand* iCurrentDealerHand)
{
    CurrentDealerHand = iCurrentDealerHand;
}

void AChessMan::CheckMovementType(ASquare* NewSquare)
{
    FSquareData lNewSquareData = NewSquare->GetData();

    if (bOnlyToUp
        || (CurrentSquare->GetData().PositionNumber.X - lNewSquareData.PositionNumber.X <= DistanceToUp.X
            && CurrentSquare->GetData().PositionNumber.Y - lNewSquareData.PositionNumber.Y <= DistanceToUp.Y
            && lNewSquareData.WarringPartiesType == EWarringPartiesType::White))
    {
        bMovementTypeToUp = true;
        bIsFigureBeingCrushed = lNewSquareData.WarringPartiesType == EWarringPartiesType::White;
    }
    else
    {
        bMovementTypeToUp = false;
        bIsFigureBeingCrushed = false;
    }

    if (lNewSquareData.WarringPartiesType == EWarringPartiesType::White)
    {
        CurrentFirstPlayer->RotateToActor(this);
    }
}

bool AChessMan::PawnTransformation()
{
    if (CurrentData.Type == EChessManType::Pawn
        && CurrentData.Position.X == 0)
    {
        CurrentData.Type = EChessManType(GetRandom(3, 1));

        if (CurrentChessManGenerator)
        {
            CurrentChessManGenerator->RemoveChessMan(this);
            CurrentChessManGenerator->CreateChessMansFromData(CurrentData);
        }

        UnsubscribeToDelegates();

        Destroy();

        return true;
    }

    return false;
}

void AChessMan::DealerHandMovementEnd()
{
    // Привязка данной Руки Дилера к компоненту Точки Захвата
    CurrentDealerHand->AttachToComponent(CapturePoint, FAttachmentTransformRules::KeepWorldTransform);

    CurrentDealerHand->bIsDragging = !bMovementTypeToUp;

    // Запуск перемещения Фигуры
    if (bMovementTypeToUp)
    {
        MovementComponent->OnCompletedMove.AddDynamic(this, &AChessMan::MovementEnd_Up);
        MovementComponent->MoveToLocation(GetActorLocation() + FVector(0.f, 0.f, LiftingHeight));
    }
    else
    {
        MovementEnd_ToSquare();
    }
}

void AChessMan::MovementEnd()
{
    if (bMovementTypeToUp)
    {
        EventOnSteppingFromAbove(bIsFigureBeingCrushed);
    }

    // Отвязать и отправить на "Базу" Руку Дилера
    CurrentDealerHand->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    CurrentDealerHand->MoveToBase();

    // Завершить перемещение и передать ход игроку
    MovementComponent->OnCompletedMove.RemoveAll(this);
    CurrentOperator->OnPlayersMove.Broadcast(true);

    if (!PawnTransformation())
    {
        // Разрешить взаимодействие с данной фигурой
        bIsMovingToNewLocation = false;

        // Повернуть фигуру в сторону игрока
        RotateToFirstPlayer();
    }
}

void AChessMan::MovementEnd_Up()
{
    MovementComponent->OnCompletedMove.RemoveAll(this);
    MovementComponent->OnCompletedMove.AddDynamic(this, &AChessMan::MovementEnd_ToSquare);
    MovementComponent->MoveToLocation(CurrentSquare->GetActorLocation() + FVector(0.f, 0.f, LiftingHeight));
}

void AChessMan::MovementEnd_ToSquare()
{
    MovementComponent->OnCompletedMove.RemoveAll(this);
    MovementComponent->OnCompletedMove.AddDynamic(this, &AChessMan::MovementEnd);
    MovementComponent->MoveToLocation(CurrentSquare->GetActorLocation());
}
//--------------------------------------------------------------------------------------



/* ---   Rotation   --- */

void AChessMan::SubscribeToDelegates()
{
    if (CurrentOperator)
    {
        // Поворот в сторону игрока с учётом выбранного типа поворота
        if (!CurrentOperator->OnPlayersMove.IsBoundToObject(this))
        {
            CurrentOperator->OnPlayersMove.AddUObject(this, &AChessMan::RotateToFirstPlayer);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s': CurrentOperator is NOT"),
            *GetNameSafe(this));
    }
}

void AChessMan::RotationInit()
{
    if (!CurrentFirstPlayer)
    {
        CurrentFirstPlayer = CurrentChessManGenerator->GetFirstPlayer();
    }

    if (!CurrentFirstPlayer)
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s': CurrentFirstPlayer is NOT"),
            *GetNameSafe(this));
    }
}

void AChessMan::RotateToFirstPlayer(const bool& bIsPlayersMove)
{
    if (!bIsPlayersMove && RotationType != EChessManRotationType::NONE)
    {
        if (CurrentFirstPlayer)
        {
            if (RotationType == EChessManRotationType::ToCharacter)
            {
                RotationComponent->RotateToLocation(CurrentFirstPlayer->GetActorLocation());
            }
            else
            {
                FVector lRotateTo;

                switch (RotationType)
                {
                case EChessManRotationType::Knight:
                    lRotateTo = GetClosestToPlayer(
                        {
                            FIntPoint { 1, 2 },
                            FIntPoint { 1, -2 },
                            FIntPoint { -1, 2 },
                            FIntPoint { -1, -2 },

                            FIntPoint { 2, 1 },
                            FIntPoint { 2, -1 },
                            FIntPoint { -2, 1 },
                            FIntPoint { -2, -1 },
                        });
                    break;

                case EChessManRotationType::Bishop:
                    lRotateTo = GetClosestToPlayer(
                        {
                            FIntPoint{ 1, 1 },
                            FIntPoint { 1, -1 },
                            FIntPoint { -1, -1 },
                            FIntPoint { -1, 1 }
                        });
                    break;

                case EChessManRotationType::Rook:
                    lRotateTo = GetClosestToPlayer(
                        {
                            FIntPoint{ 1, 0 },
                            FIntPoint { -1, 0 },
                            FIntPoint { 0, 1 },
                            FIntPoint { 0, -1 }
                        });
                    break;

                case EChessManRotationType::Queen:
                    lRotateTo = GetClosestToPlayer(
                        {
                            FIntPoint{ 1, 1 },
                            FIntPoint { 1, -1 },
                            FIntPoint { -1, -1 },
                            FIntPoint { -1, 1 },
                            FIntPoint{ 1, 0 },
                            FIntPoint { -1, 0 },
                            FIntPoint { 0, 1 },
                            FIntPoint { 0, -1 }
                        });
                    break;
                }

                RotationComponent->RotateToLocation(lRotateTo);
            }
        }
    }
}

FVector AChessMan::GetClosestToPlayer(const TArray<FIntPoint>& iVariants)
{
    FVector lResult = GetActorLocation();
    FVector lCurrentLocation = lResult;
    FVector lPlayerLocation = CurrentFirstPlayer->GetActorLocation();
    FVector lChecked;

    float lDistanceSquared = (lPlayerLocation - lResult).SizeSquared();
    // PS: SizeSquared() используется для более быстрого счёта

    for (const FIntPoint& lIndex : iVariants)
    {
        lChecked = lCurrentLocation + FVector(lIndex.X, lIndex.Y, 0) * 128;

        if (lDistanceSquared > (lPlayerLocation - lChecked).SizeSquared())
        {
            lResult = lChecked;
            lDistanceSquared = (lPlayerLocation - lResult).SizeSquared();
        }
    }

    return lResult;
}
//--------------------------------------------------------------------------------------



/* ---   Death   --- */

void AChessMan::SetCurrentChessManGenerator(AChessManGenerator* iGenerator)
{
    CurrentChessManGenerator = iGenerator;
}

void AChessMan::ChessManDeath()
{
    if (!bIsMovingToNewLocation && !bIsDead)
    {
        ChessManPreDeath();

        if (CurrentFirstPlayer)
        {
            CurrentFirstPlayer->EventOnAttack();
        }

        OnDeath.Broadcast();

        UnsubscribeToDelegates();
    }
}

void AChessMan::ChessManPreDeath()
{
    CurrentSquare->OccupySquare(EWarringPartiesType::Corpse);

    if (CurrentChessManGenerator)
    {
        CurrentChessManGenerator->RemoveChessMan(this);
    }

    bIsDead = true;
    CurrentData.bIsDead = true;

    if (ChessmanSkeletalMesh)
    {
        ChessmanSkeletalMesh->SetRenderCustomDepth(false);
        ChessmanSkeletalMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
    }

    CapturePoint->DestroyComponent();
    MovementComponent->DestroyComponent();
    RotationComponent->DestroyComponent();
}

void AChessMan::UnsubscribeToDelegates()
{
    CurrentOperator->OnPlayersMove.RemoveAll(this);
}
//--------------------------------------------------------------------------------------
