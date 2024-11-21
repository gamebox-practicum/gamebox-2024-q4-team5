//

// Base:
#include "DealerHand.h"

// Interaction:
#include "SK/ChessMans/ChessManGenerator.h"
#include "SK/Tools/ActorMovementComponent.h"
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
    MovementComponent->OnApproach.AddDynamic(this, &ADealerHand::GrabWithHand);
    MovementComponent->MoveToLocation(Point);
}

void ADealerHand::MoveToBase()
{
    OnReleaseFromHand.Broadcast();

    MovementComponent->OnCompletedMove.Clear();
    MovementComponent->OnApproach.Clear();
    MovementComponent->MoveToLocation(CurrentChessManGenerator->GetActorLocation());
}

void ADealerHand::GrabWithHand()
{
    OnGrabWithHand.Broadcast();
}
//--------------------------------------------------------------------------------------
