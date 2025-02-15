// 

#pragma once

// Base:
#include "CoreMinimal.h"

// Generated:
#include "TimeBeaconGenerator.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// Interaction:
class ATimeBeacon;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API ATimeBeaconGenerator : public AActor
{
    GENERATED_BODY()

public:


    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    ATimeBeaconGenerator();
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    /** Overridable native event for when play begins for this actor */
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Re Generate   --- */

    /** Перегенерировать (перезапустить) данный Генератор */
    UFUNCTION(BlueprintCallable, Category = "Settings", CallInEditor)
    void ReGenerate();
    //-------------------------------------------



    /* ---   Generator   --- */

    // Тип Маяка времени (Колонны)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Generator")
    TSubclassOf<ATimeBeacon> BeaconType;

    // Количество клеток доски вдоль осей
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generator")
    FIntPoint NumberOfSquaresAlongAxes = { 10, 10 };

    // Вычисленный размер блоков Клеток, на которые ориентируется данный генератор
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    FVector BlockSize = FVector::ZeroVector;

    //

    /** Установить таймер для Маяков */
    void SetTimeForBeacons(const float& Time);

    /** Остановить и сбросить все Маяки */
    void StopTimeBeaconGenerator();

    /** Запустить таймер Генератора Маяков */
    void PlayTimeBeaconGenerator();

    /** Обновить этап на всех колоннах */
    void UpdateStage(const int32& CurrentStageNum);
    //-------------------------------------------



private:

    /* ---   Re Generate   --- */

    // Тег для определения клетки, созданной генератором
    FName VerificationTag = FName(GetNameSafe(this));

    //

    /** Удалить все созданные Маяки */
    void DeleteAllTimeBeacons();

    /** Создать Маяки */
    void CreateGeneratedTimeBeacons();

    /** Создать Маяк с указанным индексом по оси X */
    ATimeBeacon* CreateTimeBeacon(const int32& X);
    //-------------------------------------------



    /* ---   Generator   --- */

    // Массив указателей на все Маяки
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    TArray<ATimeBeacon*> AllTimeBeacon;

    //

    /** Получить локацию Маяка с указанным индексом массива */
    FVector GetLocationForTimeBeacon(const int32& Ind);
    //-------------------------------------------



    /* ---   Time control   --- */

    // Таймер-Счётчик для последовательного выбора Колон
    FTimerHandle Timer_Counter;

    // Счётчик таймера
    int32 CounterForTimer = 0;

    // Рассчитанный кусок от общего времени
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    float PartOfTime = 0.f;

    //

    /** Реакция таймера: Передача хода Оператору */
    void TimerAction_TriggerForBeacon();
    //-------------------------------------------
};
