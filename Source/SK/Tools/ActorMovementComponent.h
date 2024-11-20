// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// C++:
#include <functional>

// Generated:
#include "ActorMovementComponent.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Delegates   --- */

// Делегат хода Игроков
DECLARE_DELEGATE(FOnCompletedMove);
// ----------------------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// UE:
class AActor;
//--------------------------------------------------------------------------------------



UCLASS(ClassGroup = (Custom), BlueprintType, Blueprintable, EditInlineNew,
    meta = (BlueprintSpawnableComponent))
    class SK_API UActorMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    /* ---   Delegates   --- */

    FOnCompletedMove OnCompletedMove; // Делегат завершения перемещения
    // ----------------------------------------------------------------------------------------------------



    /* ---   Constructors   --- */

    // Sets default values for this component's properties
    UActorMovementComponent();
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Called when the game starts
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Base   --- */

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Вызывается при создании компонента */
    virtual void OnComponentCreated() override;

    /** Вызывается при уничтожении компонента
    @param bDestroyingHierarchy - значение True, если разрушается вся иерархия компонентов.
    Позволяет избежать дорогостоящих операций
    */
    virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
    //-------------------------------------------



    /* ---   Actor Movement   --- */

    // Флаг контроля плавности скорости на старте движения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Actor Movement")
    bool bControlSpeedAtStart = true;

    // Максимальная скорость перемещения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Actor Movement")
    float MaxSpeed = 1000.f;

    // Минимальный шаг перемещения.
    // Если до конечной точки осталось расстояние меньше MinStep
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Actor Movement")
    float MinStep = 0.5f;

    // Коэффициент ускорения скорости перемещения в начале пути
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Actor Movement")
    float AccelerationCoefficient = 10.f;

    // Коэффициент замедления скорости перемещения в конце пути
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Actor Movement")
    float DecelerationCoefficient = 10.f;

    //

    /** Переместиться к указанной Клетке
    */
    void MoveToLocation(const FVector& Point);
    //-------------------------------------------



private:

    /* ---   This Component   --- */

    // Текущий Актор-владелец
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    AActor* CurrentActor = nullptr;

    //

    /** Инициализация данных текущего компонента*/
    void CurrentComponentInit();
    //-------------------------------------------



    /* ---   Actor Movement   --- */

    // Флаг контроля перемещения
    bool bIsMovingToNewLocation = false;

    // Стартовая локация (начало) перемещения
    FVector StartLocation;

    // Конечная локация перемещения
    FVector EndLocation;

    //

    /** Расчёт передвижения на каждый кадр */
    void MovementForTick(const float& DeltaTime);
    //-------------------------------------------
};
