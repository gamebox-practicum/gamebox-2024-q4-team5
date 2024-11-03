// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Tools:


// Generated:
#include "Square.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// Interaction:
struct FSquareData;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API ASquare : public AActor
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    ASquare();
    //-------------------------------------------



    /* ---   Components   --- */

    /** Меш визуализации блока */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* BlockMesh = nullptr;
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Interaction   --- */

    /** Событие, когда на него наводят курсор мыши с помощью интерактивного интерфейса */
    virtual void NotifyActorBeginCursorOver() override;

    /** Событие, когда с него убирают курсор мыши с помощью интерактивного интерфейса */
    virtual void NotifyActorEndCursorOver() override;
    //-------------------------------------------



    /* ---   Generator   --- */

    // Материал клетки: Тип A
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
    UMaterialInterface* BlockMaterialsA;

    // Материал клетки: Тип B
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
    UMaterialInterface* BlockMaterialsB;

    //

    // Установить и применить настройки по Данным клетки
    void SetData(FSquareData Data);
    //-------------------------------------------



private:

    /* ---   Generator   --- */

    // Установка материала по номеру
    void SetMaterialByType(int32& NumType);
    //-------------------------------------------
};
