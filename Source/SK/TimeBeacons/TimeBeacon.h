// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Generated:
#include "TimeBeacon.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// Interaction:
class AChessOperator;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API ATimeBeacon : public AActor
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    ATimeBeacon();
    //-------------------------------------------



    /* ---   Components   --- */

    // Меш Колонны со скелетом
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
    //    meta = (AllowPrivateAccess = "true"))
    //USkeletalMeshComponent* ColumnSkeletalMesh;

    // Статичный Меш Колонны
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* ColumnStaticMesh;
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Timer   --- */

    /** Записать значения времени для Таймера */
    void SetTimeForTimer(const float& Time);

    /** Запустить Таймер */
    void PlayTimer();

    /** Остановить Таймер */
    void StopTimer();
    //-------------------------------------------



    /* ---   Events   --- */

    /** Событие Записи значения времени для Таймера */
    UFUNCTION(BlueprintImplementableEvent, Category = "Timer",
        meta = (DisplayName = "Set Time For Timer"))
    void EventSetTimeForTimer(const float& Time);

    /** Событие Запуска Таймера */
    UFUNCTION(BlueprintImplementableEvent, Category = "Timer",
        meta = (DisplayName = "Play Timer"))
    void EventPlayTimer();

    /** Событие Останова Таймера */
    UFUNCTION(BlueprintImplementableEvent, Category = "Timer",
        meta = (DisplayName = "Stop Timer"))
    void EventStopTimer();

    UFUNCTION(BlueprintImplementableEvent, Category = "Timer",
        meta = (DisplayName = "On Next Stage"))
    void EventOnNextStage(const int32& StageNum);
    //-------------------------------------------



private:

    /* ---   Base   --- */

    /** Очистка от неиспользуемых компонентов */
    void Cleaning();
    //-------------------------------------------
};
