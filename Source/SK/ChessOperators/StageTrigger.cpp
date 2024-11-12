// 

// Base:
#include "StageTrigger.h"

// UE:
#include "Components/BoxComponent.h"

// Interaction:
#include "SK/Core/SK_Character.h"
#include "SK/ChessOperators/ChessOperator.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values
AStageTrigger::AStageTrigger()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Preliminarily



    /* ---   Components   --- */

    // Корневой компонент
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Компонент коллизии
    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
    BoxCollision->SetupAttachment(RootComponent);
    BoxCollision->SetBoxExtent(FVector(100));
    BoxCollision->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

// Called when the game starts or when spawned
void AStageTrigger::BeginPlay()
{
    Super::BeginPlay();

}
//--------------------------------------------------------------------------------------



/* ---   Collision   --- */

void AStageTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (Cast<ASK_Character>(OtherActor))
    {
        Destroy();

        CurrentOperator->ToNextStage();
    }
}
//--------------------------------------------------------------------------------------



/* ---   Trigger   --- */

void AStageTrigger::SetPointerToOperator(AChessOperator* iCurrentOperator)
{
    CurrentOperator = iCurrentOperator;
}
//--------------------------------------------------------------------------------------
