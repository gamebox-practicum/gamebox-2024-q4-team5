// 

// Base:
#include "SquareComponent.h"

// UE:
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values for this component's properties
USquareComponent::USquareComponent()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryComponentTick.bCanEverTick = false; // Предварительно

    bAutoActivate = true;
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

// Called when the game starts
void USquareComponent::BeginPlay()
{
    Super::BeginPlay();

    ReactionInit();
}

// Called every frame
void USquareComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USquareComponent::OnComponentCreated()
{
    Super::OnComponentCreated();

    CurrentComponentInit();

    ReactionPreInit();
    CreateActorOverSquare();
}

void USquareComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
    DestroyReaction();
    DestroyCreatedActor();

    Super::OnComponentDestroyed(bDestroyingHierarchy);
}
//--------------------------------------------------------------------------------------



/* ---   Reactions   --- */

void USquareComponent::CurrentComponentInit()
{
    CurrentSquare = Cast<ASquare>(GetOwner());

    if (!CurrentSquare)
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': CurrentSquare is NOT"),
            *GetNameSafe(this));
    }
}

void USquareComponent::ReactionPreInit()
{
    if (bEnableReactions
        && CurrentSquare
        && !ResponseBoxComponent)
    {
        // Создать и получить компонент реагирования
        ResponseBoxComponent = Cast<UBoxComponent>(
            CurrentSquare->AddComponentByClass(UBoxComponent::StaticClass(), false, FTransform(), false));

        if (ResponseBoxComponent)
        {
            CurrentSquare->AddInstanceComponent(ResponseBoxComponent);
            ResponseBoxComponent->RegisterComponent();
        }
    }
    else
    {
        DestroyReaction();
    }
}

void USquareComponent::ReactionInit()
{
    if (ResponseBoxComponent)
    {
        ResponseBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &USquareComponent::ReactionToBeginOverlap);
        ResponseBoxComponent->OnComponentEndOverlap.AddDynamic(this, &USquareComponent::ReactionToEndOverlap);
    }
}

void USquareComponent::DestroyReaction()
{
    if (ResponseBoxComponent)
    {
        ResponseBoxComponent->DestroyComponent();
        ResponseBoxComponent = nullptr;
    }
}

void USquareComponent::ReactionToBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    EventReactionToBeginOverlap(
        OverlappedComponent,
        OtherActor,
        OtherComp,
        OtherBodyIndex,
        bFromSweep,
        SweepResult);
}

void USquareComponent::ReactionToEndOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    EventReactionToEndOverlap(
        OverlappedComponent,
        OtherActor,
        OtherComp,
        OtherBodyIndex);
}
//--------------------------------------------------------------------------------------



/* ---   Actor Creation   --- */

void USquareComponent::CreateActorOverSquare()
{
    if (bCreatesActor)
    {
        if (CurrentSquare
            && CreatedActorType)
        {
            CurrentSquare->OccupySquare(PartiesType);

            if (!CreatedActor
                || CreatedActor->StaticClass() != CreatedActorType.Get())
            {
                CreatedActor = GetWorld()->SpawnActor<AActor>(
                    CreatedActorType.Get(),
                    CurrentSquare->GetActorLocation(),
                    FRotator::ZeroRotator);
            }
        }
        else if (!CurrentSquare)
        {
            UE_LOG(LogTemp, Error, TEXT("'%s': CurrentSquare is NOT"),
                *GetNameSafe(this));
        }
        else if (!CreatedActorType)
        {
            UE_LOG(LogTemp, Error, TEXT("'%s': CreatedActorType is NOT"),
                *GetNameSafe(this));
        }
    }
    else
    {
        DestroyCreatedActor();
    }
}

void USquareComponent::DestroyCreatedActor()
{
    if (CreatedActor)
    {
        CreatedActor->Destroy();
        CreatedActor = nullptr;
    }
}
//--------------------------------------------------------------------------------------
