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

    // Статичный Меш Шахматной Фигуры
    ChessmanStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chessman Static Mesh"));
    ChessmanStaticMesh->SetupAttachment(RootComponent);
    ChessmanStaticMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    ChessmanStaticMesh->SetCustomDepthStencilValue(1);

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

void AChessMan::Cleaning()
{
    if (ChessmanSkeletalMesh && !(ChessmanSkeletalMesh->SkeletalMesh))
    {
        ChessmanSkeletalMesh->DestroyComponent();
        CapsuleComponent->DestroyComponent();
    }

    if (ChessmanStaticMesh && !(ChessmanStaticMesh->GetStaticMesh()))
    {
        ChessmanStaticMesh->DestroyComponent();
    }
}

void AChessMan::Initialize()
{
    RotationInit();
    SubscribeToDelegates();

    /* ---   По завершении создания и инициализации:   --- */

    // Задействовать событие завершении Инициализации
    EventOnInitializeComplete();

    // Повернуть в сторону игрока
    RotateToFirstPlayer();
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
        if (ChessmanStaticMesh)
            ChessmanStaticMesh->SetRenderCustomDepth(true);
    }
}

void AChessMan::NotifyActorEndCursorOver()
{
    Super::NotifyActorEndCursorOver();

    if (ChessmanSkeletalMesh)
        ChessmanSkeletalMesh->SetRenderCustomDepth(false);
    if (ChessmanStaticMesh)
        ChessmanStaticMesh->SetRenderCustomDepth(false);
}

void AChessMan::NotifyActorOnClicked(FKey ButtonReleased)
{
    Super::NotifyActorOnClicked(ButtonReleased);

    ChessManDeath();
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

        SetCurrentSquare(ToSquare);

        // Сохранение данных
        CurrentData.Position = CurrentSquare->GetData().PositionNumber;

        // Запуск перемещения Руки Дилера
        CurrentDealerHand->MovementComponent->OnCompletedMove.AddDynamic(this, &AChessMan::DealerHandMovementEnd);
        CurrentDealerHand->MoveToLocation(CapturePoint->GetComponentLocation());
    }
}

void AChessMan::SetCurrentSquare(ASquare* ToSquare)
{
    // Освободить предыдущую клетку и занять новую
    if (CurrentSquare)
        CurrentSquare->OccupySquare(EWarringPartiesType::NONE);
    ToSquare->OccupySquare(EWarringPartiesType::Black);

    CurrentSquare = ToSquare;
}

void AChessMan::SetPointerToOperator(AChessOperator* iCurrentOperator)
{
    CurrentOperator = iCurrentOperator;
}

void AChessMan::SetCurrentDealerHand(ADealerHand* iCurrentDealerHand)
{
    CurrentDealerHand = iCurrentDealerHand;
}

void AChessMan::DealerHandMovementEnd()
{
    // Привязка данной Руки Дилера к компоненту Точки Захвата
    CurrentDealerHand->AttachToComponent(CapturePoint, FAttachmentTransformRules::KeepWorldTransform);

    // Запуск перемещения Фигуры
    MovementComponent->OnCompletedMove.AddDynamic(this, &AChessMan::MovementEnd);
    MovementComponent->MoveToLocation(CurrentSquare->GetActorLocation());
}

void AChessMan::MovementEnd()
{
    // Отвязать и отправить на "Базу" Руку Дилера
    CurrentDealerHand->DetachRootComponentFromParent();
    CurrentDealerHand->MoveToBase();

    // Завершить перемещение и передать ход игроку
    MovementComponent->OnCompletedMove.Clear();
    CurrentOperator->OnPlayersMove.Broadcast(true);

    // Разрешить взаимодействие с данной фигурой
    bIsMovingToNewLocation = false;

    // Повернуть фигуру в сторону игрока
    RotateToFirstPlayer();
}
//--------------------------------------------------------------------------------------



/* ---   Rotation   --- */

void AChessMan::SubscribeToDelegates()
{
    if (CurrentOperator)
    {
        // Поворот в сторону игрока с учётом выбранного типа поворота
        CurrentOperator->OnPlayersMove.AddUObject(this, &AChessMan::RotateToFirstPlayer);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': CurrentOperator is NOT"),
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
                            FIndex2D { 1, 2 },
                            FIndex2D { 1, -2 },
                            FIndex2D { -1, 2 },
                            FIndex2D { -1, -2 },

                            FIndex2D { 2, 1 },
                            FIndex2D { 2, -1 },
                            FIndex2D { -2, 1 },
                            FIndex2D { -2, -1 },
                        });
                    break;

                case EChessManRotationType::Bishop:
                    lRotateTo = GetClosestToPlayer(
                        {
                            FIndex2D{ 1, 1 },
                            FIndex2D { 1, -1 },
                            FIndex2D { -1, -1 },
                            FIndex2D { -1, 1 }
                        });
                    break;

                case EChessManRotationType::Rook:
                    lRotateTo = GetClosestToPlayer(
                        {
                            FIndex2D{ 1, 0 },
                            FIndex2D { -1, 0 },
                            FIndex2D { 0, 1 },
                            FIndex2D { 0, -1 }
                        });
                    break;

                case EChessManRotationType::Queen:
                    lRotateTo = GetClosestToPlayer(
                        {
                            FIndex2D{ 1, 1 },
                            FIndex2D { 1, -1 },
                            FIndex2D { -1, -1 },
                            FIndex2D { -1, 1 },
                            FIndex2D{ 1, 0 },
                            FIndex2D { -1, 0 },
                            FIndex2D { 0, 1 },
                            FIndex2D { 0, -1 }
                        });
                    break;
                }

                RotationComponent->RotateToLocation(lRotateTo);
            }
        }
    }
}

FVector AChessMan::GetClosestToPlayer(const TArray<FIndex2D>& iVariants)
{
    FVector lResult = GetActorLocation();
    FVector lCurrentLocation = lResult;
    FVector lPlayerLocation = CurrentFirstPlayer->GetActorLocation();
    FVector lChecked;

    float lDistanceSquared = (lPlayerLocation - lResult).SizeSquared();
    // PS: SizeSquared() используется для более быстрого счёта

    for (const FIndex2D& lIndex : iVariants)
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
        if (CurrentChessManGenerator)
        {
            CurrentChessManGenerator->RemoveChessMan(this);
        }

        CurrentSquare->OccupySquare(EWarringPartiesType::Corpse);

        for (auto& lSquareComponent : SquareComponentsTypes)
        {
            // Создаём компонент, который всё остальное сделает сам
            Cast<USquareComponent>(
                CurrentSquare->AddComponentByClass(
                    lSquareComponent.Get(),
                    false,
                    FTransform(),
                    false));
        }

        bIsDead = true;

        if (ChessmanSkeletalMesh)
            ChessmanSkeletalMesh->SetRenderCustomDepth(false);
        if (ChessmanStaticMesh)
            ChessmanStaticMesh->SetRenderCustomDepth(false);

        UnsubscribeToDelegates();

        OnDeath.Broadcast();
    }
}

void AChessMan::UnsubscribeToDelegates()
{
    CurrentOperator->OnPlayersMove.RemoveAll(this);
}
//--------------------------------------------------------------------------------------
