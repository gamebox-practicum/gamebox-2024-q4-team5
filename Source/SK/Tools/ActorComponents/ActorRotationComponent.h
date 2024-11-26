// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// Generated:
#include "ActorRotationComponent.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Delegates   --- */

// Делегат: Завершение вращения
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompletedRotate);
// ----------------------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Interaction:
class AChessMan;
//--------------------------------------------------------------------------------------



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SK_API UActorRotationComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    /* ---   Delegates   --- */

    // Делегат: Завершение вращения
    UPROPERTY(BlueprintAssignable)
    FOnCompletedRotate OnCompletedRotate;
    //-------------------------------------------



    /* ---   Constructors   --- */

    // Sets default values for this component's properties
    UActorRotationComponent();
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



    /* ---   ChessMan Rotate   --- */

    // Флаг контроля вращения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Rotation")
    bool bIsRotatedToNewRotation = false;

    // Максимальная скорость вращения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Rotation|Speed")
    float MaxSpeed = 100.f;

    // Минимальный шаг вращения.
    // Если до конечного значения вращения осталось меньше MinStep, то Шахматной Фигуре присваивается необходимая ротация
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Rotation|Speed")
    float MinStep = 0.5f;

    // Флаг контроля плавности скорости на старте вращения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Rotation|Acceleration")
    bool bControlSpeedAtStart = true;

    // Коэффициент ускорения скорости вращения в начале пути
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Rotation|Acceleration")
    float AccelerationCoefficient = 10.f;

    // Коэффициент замедления скорости вращения в конце пути
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Rotation|Acceleration")
    float DecelerationCoefficient = 10.f;

    //

    /** Повернуться к указанной Точке */
    void RotateToLocation(const FVector& Point);
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



    /* ---   ChessMan Rotate   --- */

    // Стартовая ротация (начало) вращения
    float StartRotation;

    // Конечная ротация вращения
    float EndRotation;

    //

    /** Расчёт вращения на каждый кадр */
    void RotationForTick(const float& DeltaTime);
    //-------------------------------------------
};
