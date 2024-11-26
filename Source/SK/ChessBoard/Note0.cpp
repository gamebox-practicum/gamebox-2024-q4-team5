// Fill out your copyright notice in the Description page of Project Settings.


#include "Note0.h"

#include "SK/Core/SK_Character.h"


// Sets default values
ANote0::ANote0()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Block Mesh"));
    Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANote0::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ANote0::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

/* ---   Interaction   --- */

void ANote0::NotifyActorBeginCursorOver()
{
    Super::NotifyActorBeginCursorOver();

    if (!GetWorld()->IsPaused() && IsCloseEnough())
    {
        Mesh->SetRenderCustomDepth(true);
    }
}

void ANote0::NotifyActorEndCursorOver()
{
    Super::NotifyActorEndCursorOver();

    Mesh->SetRenderCustomDepth(false);
}

void ANote0::NotifyActorOnClicked(FKey ButtonReleased)
{
    Super::NotifyActorOnClicked(ButtonReleased);

    if(IsCloseEnough())
    {
        OnPicked();

        Destroy();
    }
}

bool ANote0::IsCloseEnough()
{
    return
    FVector::Distance(
        Cast<ASK_Character>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetActorLocation(),
        GetActorLocation()) < PickMaxDistance;
}

