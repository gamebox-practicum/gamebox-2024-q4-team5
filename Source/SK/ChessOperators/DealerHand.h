// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// Generated:
#include "DealerHand.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Delegates   --- */

// Делегат: При Готовности захватывать рукой
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReadyToGrabWithHand);

// Делегат: При Захвате рукой
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrabWithHand);

// Делегат: При Освобождении из рук
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleaseFromHand);
// ----------------------------------------------------------------------------------------------------



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

    /* ---   Delegates   --- */

    // Делегат: При Захвате рукой
    UPROPERTY(BlueprintAssignable)
    FOnReadyToGrabWithHand OnReadyToGrabWithHand;

    // Делегат: При Захвате рукой
    UPROPERTY(BlueprintAssignable)
    FOnGrabWithHand OnGrabWithHand;

    // Делегат: При Освобождении из рук
    UPROPERTY(BlueprintAssignable)
    FOnReleaseFromHand OnReleaseFromHand;
    //-------------------------------------------



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

    /* ---   Components   --- */

    // Called every frame
    virtual void Tick(float DeltaTime) override;
    //-------------------------------------------



    /* ---   Movement   --- */

    // Рука волочет фигуру по доске
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
    bool bIsDragging = true;

    //

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

    // Изначальное значение: Флаг контроля плавности скорости на старте движения
    bool bOriginalMeaning_ControlSpeedAtStart = false;

    //

    /** Событие: Захватить рукой */
    UFUNCTION()
    void ReadyToGrabWithHand();

    /** Событие: Захватить рукой */
    UFUNCTION()
    void GrabWithHand();

    /** Событие: Сбросить ротацию */
    UFUNCTION()
    void ResetRotation();
    //-------------------------------------------
};
