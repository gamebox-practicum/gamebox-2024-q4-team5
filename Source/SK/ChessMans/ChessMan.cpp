//

// Base:
#include "ChessMan.h"

// Interaction:
#include "ChessManGenerator.h"
#include "SK/ChessBoard/Square.h"
#include "SK/ChessBoard/SquareComponent.h"
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

    // Меш Шахматной Фигуры со скелетом
    ChessmanSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Chessman Skeletal Mesh"));
    ChessmanSkeletalMesh->SetupAttachment(RootComponent);

    // Статичный Меш Шахматной Фигуры
    ChessmanStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chessman Static Mesh"));
    ChessmanStaticMesh->SetupAttachment(RootComponent);
    ChessmanStaticMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
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

    MovementForTick(DeltaTime);
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
        // Освободить предыдущую клетку и занять новую
        if (CurrentSquare)
            CurrentSquare->OccupySquare(EWarringPartiesType::NONE);
        ToSquare->OccupySquare(EWarringPartiesType::Black);

        CurrentSquare = ToSquare;

        NewLocation = ToSquare->GetActorLocation();
        NewLocation.Z = GetActorLocation().Z;

        CurrentData.Position = ToSquare->GetData().PositionNumber;

        bIsMovingToNewLocation = true;
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

void AChessMan::MovementForTick(const float& lDeltaTime)
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
        }
        else
        {
            // Плавная интерполяция перемещения
            SetActorLocation(FMath::VInterpTo(lCurrentLocation, NewLocation, lDeltaTime, MovementSpeed));
        }
    }
}
//--------------------------------------------------------------------------------------



/* ---   Death   --- */

void AChessMan::SetCurrentChessManGenerator(AChessManGenerator* Generator)
{
    CurrentChessManGenerator = Generator;
}

void AChessMan::ChessManDeath()
{
    Destroy();

    // Указатель на создаваемый компонент
    USquareComponent* lNewComponent = nullptr;

    for (auto& lSquareComponent : SquareComponentsTypes)
    {
        // Создаём компонент, который всё остальное сделает сам
        lNewComponent = Cast<USquareComponent>(
            CurrentSquare->AddComponentByClass(
                lSquareComponent.Get(),
                false,
                FTransform(),
                false));
    }

    if (CurrentChessManGenerator)
    {
        CurrentChessManGenerator->UpdateAllChessMan();
    }
}
//--------------------------------------------------------------------------------------
