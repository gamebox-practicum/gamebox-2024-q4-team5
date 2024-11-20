//

// Base:
#include "ChessMan.h"

// UE:
#include "Components/CapsuleComponent.h"

// Interaction:
#include "ChessManGenerator.h"
#include "SK/ChessBoard/Square.h"
#include "SK/ChessBoard/SquareComponent.h"
#include "SK/Tools/ActorMovementComponent.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values
AChessMan::AChessMan()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true; // Warning: Принудительно!



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

    // Статичный Меш Шахматной Фигуры
    ChessmanStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chessman Static Mesh"));
    ChessmanStaticMesh->SetupAttachment(RootComponent);
    ChessmanStaticMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

    // Компонент перемещения данного Актора
    MovementComponent = CreateDefaultSubobject<UActorMovementComponent>(TEXT("Movement Component"));
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
//--------------------------------------------------------------------------------------



/* ---   Interaction   --- */

void AChessMan::NotifyActorBeginCursorOver()
{
    Super::NotifyActorBeginCursorOver();

    if (GetWorldSettings()->GetPauserPlayerState() != NULL)
    {
        return;
    }

    if (ChessmanSkeletalMesh)
        ChessmanSkeletalMesh->SetRenderCustomDepth(true);
    if (ChessmanStaticMesh)
        ChessmanStaticMesh->SetRenderCustomDepth(true);
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



/* ---   Movement   --- */

// Warning: Предварительный вариант.
// В будущем планируется переделать в имитацию "переноса" рукой "Дилера"

void AChessMan::MoveToSquare(ASquare* ToSquare)
{
    if (!bIsMovingToNewLocation && ToSquare)
    {
        bIsMovingToNewLocation = true;

        // Освободить предыдущую клетку и занять новую
        if (CurrentSquare)
            CurrentSquare->OccupySquare(EWarringPartiesType::NONE);
        ToSquare->OccupySquare(EWarringPartiesType::Black);

        // Сохранение данных
        CurrentSquare = ToSquare;
        CurrentData.Position = CurrentSquare->GetData().PositionNumber;

        // Запуск перемещения
        MovementComponent->OnCompletedMove.BindUObject(this, &AChessMan::MovementEnd);
        MovementComponent->MoveToLocation(ToSquare->GetActorLocation());
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

void AChessMan::MovementEnd()
{
    MovementComponent->OnCompletedMove.Unbind();

    bIsMovingToNewLocation = false;
}
//--------------------------------------------------------------------------------------



/* ---   Death   --- */

void AChessMan::SetCurrentChessManGenerator(AChessManGenerator* iGenerator)
{
    CurrentChessManGenerator = iGenerator;
}

void AChessMan::ChessManDeath()
{
    if (!bIsMovingToNewLocation)
    {
        if (CurrentChessManGenerator)
        {
            CurrentChessManGenerator->RemoveChessMan(this);
        }

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

        Destroy();
    }
}
//--------------------------------------------------------------------------------------
