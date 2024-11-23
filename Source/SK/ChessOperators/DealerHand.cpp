//

// Base:
#include "DealerHand.h"

// Interaction:
#include "SK/ChessMans/ChessManGenerator.h"
#include "SK/Tools/ActorComponents/ActorMovementComponent.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values
ADealerHand::ADealerHand()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Предварительно



    /* ---   Components   --- */

    // Корневой компонент
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Меш Шахматной Фигуры со скелетом
    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
    SkeletalMesh->SetupAttachment(RootComponent);

    // Компонент перемещения данного Актора
    MovementComponent = CreateDefaultSubobject<UActorMovementComponent>(TEXT("Movement Component"));
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

// Called when the game starts or when spawned
void ADealerHand::BeginPlay()
{
    Super::BeginPlay();

    bOriginalMeaning_ControlSpeedAtStart = MovementComponent->bControlSpeedAtStart;
}

// Called every frame
void ADealerHand::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
//--------------------------------------------------------------------------------------



/* ---   Movement   --- */

void ADealerHand::SetCurrentChessManGenerator(AChessManGenerator* iGenerator)
{
    CurrentChessManGenerator = iGenerator;
}

void ADealerHand::MoveToLocation(const FVector& Point)
{
    if (bOriginalMeaning_ControlSpeedAtStart)
    {
        MovementComponent->bControlSpeedAtStart = false;
    }
    
    MovementComponent->OnApproach.AddDynamic(this, &ADealerHand::GrabWithHand);
    MovementComponent->MoveToLocation(Point);
}

void ADealerHand::MoveToBase()
{
    OnReleaseFromHand.Broadcast();

    if (bOriginalMeaning_ControlSpeedAtStart)
    {
        MovementComponent->bControlSpeedAtStart = true;
    }

    MovementComponent->OnCompletedMove.Clear();
    MovementComponent->OnApproach.Clear();
    MovementComponent->MoveToLocation(CurrentChessManGenerator->GetActorLocation());

    if (GetActorRotation() != FRotator::ZeroRotator
        && GetActorLocation() == CurrentChessManGenerator->GetActorLocation())
    {
        SetActorRotation(FRotator::ZeroRotator);
    }
}

void ADealerHand::GrabWithHand()
{
    OnGrabWithHand.Broadcast();
}
//--------------------------------------------------------------------------------------
