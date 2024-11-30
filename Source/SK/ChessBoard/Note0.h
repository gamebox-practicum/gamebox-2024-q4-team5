// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Note0.generated.h"

UCLASS()
class SK_API ANote0 : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ANote0();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* Mesh = nullptr;

    UPROPERTY(BlueprintReadWrite)
    float PickMaxDistance = 2000.0f;

    /* ---   Interaction   --- */

    /** Событие, когда на него наводят курсор мыши с помощью интерактивного интерфейса */
    virtual void NotifyActorBeginCursorOver() override;

    /** Событие, когда с него убирают курсор мыши с помощью интерактивного интерфейса */
    virtual void NotifyActorEndCursorOver() override;

    /** Событие, когда этот субъект находится под мышкой при нажатии клавиши реагирования с помощью интерактивного интерфейса */
    virtual void NotifyActorOnClicked(FKey ButtonReleased) override;
    //-------------------------------------------

    UFUNCTION(BlueprintImplementableEvent)
    void OnPicked();

private:
    bool IsCloseEnough();
};
