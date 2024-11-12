// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Generated:
#include "StageTrigger.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// Interaction:
class AChessOperator;
class UBoxComponent;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API AStageTrigger : public AActor
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    AStageTrigger();
    //-------------------------------------------



    /* ---   Components   --- */

    /** Компонент коллизии данного Триггера */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UBoxComponent* BoxCollision = nullptr;
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Collision   --- */

    /**	Событие, когда этот субъект перекрывается с другим */
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    //-------------------------------------------



    /* ---   Trigger   --- */

    /** Установить указатель на текущий Оператор */
    void SetPointerToOperator(AChessOperator* CurrentOperator);
    //-------------------------------------------



private:

    /* ---   Trigger   --- */

    // Указатель на оператора
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    AChessOperator* CurrentOperator = nullptr;

    //


    //-------------------------------------------
};
