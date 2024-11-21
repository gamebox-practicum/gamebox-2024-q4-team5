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

// Делегат: Ход Игроков
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompletedMove);

// Делегат: На Приближении
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnApproach);

// Делегат: На Отдалении
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSeparation);
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

    UPROPERTY(BlueprintAssignable)
    FOnCompletedMove OnCompletedMove; // Делегат: Ход Игроков

    UPROPERTY(BlueprintAssignable)
    FOnApproach OnApproach;           // Делегат: На Приближении

    UPROPERTY(BlueprintAssignable)
    FOnSeparation OnSeparation;       // Делегат: На Отдалении
    //-------------------------------------------



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

    // Максимальная скорость перемещения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement|Speed")
    float MaxSpeed = 1000.f;

    // Минимальный шаг перемещения.
    // Если до конечной точки осталось расстояние меньше MinStep
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement|Speed")
    float MinStep = 0.5f;

    // Флаг контроля плавности скорости на старте движения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement|Acceleration")
    bool bControlSpeedAtStart = true;

    // Коэффициент ускорения скорости перемещения в начале пути
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement|Acceleration")
    float AccelerationCoefficient = 10.f;

    // Коэффициент замедления скорости перемещения в конце пути
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement|Acceleration")
    float DecelerationCoefficient = 10.f;

    // Расстояние, на котором срабатывает Делегат OnApproach (На Приближении)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement|Distance Triggers")
    float ApproachDistance = 100.f;

    // Расстояние, на котором срабатывает Делегат OnSeparation (На Отдалении)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement|Distance Triggers")
    float SeparationDistance = 100.f;

    //

    /** Переместиться к указанной Точке */
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

    // Флаг срабатывания Делегата OnApproach (На Приближении)
    bool bApproachWork = false;

    // Флаг срабатывания Делегата OnSeparation (На Отдалении)
    bool bSeparationWork = false;

    //

    /** Расчёт передвижения на каждый кадр */
    void MovementForTick(const float& DeltaTime);
    //-------------------------------------------
};
