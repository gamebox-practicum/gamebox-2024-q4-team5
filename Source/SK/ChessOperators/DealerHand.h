// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// Generated:
#include "DealerHand.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// Interaction:
class AChessManGenerator;
class UActorMovementComponent;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API ADealerHand : public APawn
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    ADealerHand();
    //-------------------------------------------



    /* ---   Components   --- */

    // Меш Руки со скелетом
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SkeletalMesh;

    // Компонент перемещения данного Актора
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UActorMovementComponent* MovementComponent;
    //-------------------------------------------



protected:

    /* ---   Components   --- */

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Events   --- */

    /** Событие: Захватить рукой */
    UFUNCTION(BlueprintImplementableEvent, Category = "Hand Events",
        meta = (DisplayName = "Grab with Hand"))
    void EventGrabWithHand();

    /** Событие: Освободить рукой */
    UFUNCTION(BlueprintImplementableEvent, Category = "Hand Events",
        meta = (DisplayName = "Release from Hand"))
    void EventReleaseFromHand();
    //-------------------------------------------



    /* ---   Components   --- */

    // Called every frame
    virtual void Tick(float DeltaTime) override;
    //-------------------------------------------



    /* ---   Movement   --- */

    /** Установить указатель на текущий Генератор Шахматных фигур */
    void SetCurrentChessManGenerator(AChessManGenerator* Generator);

    /** Переместиться к указанной Точке */
    void MoveToLocation(const FVector& Point);

    /** Переместиться на "Базу", т.е. на локацию Актора текущего Генератора Шахматных фигур */
    void MoveToBase();
    //-------------------------------------------



private:

    /* ---   Movement   --- */

    // Указатель на текущий Генератор Шахматных фигур
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    AChessManGenerator* CurrentChessManGenerator = nullptr;
    //-------------------------------------------
};
