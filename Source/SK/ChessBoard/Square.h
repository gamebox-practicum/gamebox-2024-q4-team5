// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Structs:
#include "SquareStruct.h"

// Generated:
#include "Square.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// Interaction:
class ASK_Character;
class ASquareGenerator;
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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
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

    /** Событие, когда этот субъект находится под мышкой при нажатии клавиши реагирования с помощью интерактивного интерфейса */
    virtual void NotifyActorOnClicked(FKey ButtonReleased) override;
    //-------------------------------------------



    /* ---   Generator   --- */

    // Материал клетки: Тип A
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
    UMaterialInterface* BlockMaterialsA;

    // Материал клетки: Тип B
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
    UMaterialInterface* BlockMaterialsB;

    //

    /** Получить номер типа материала */
    int32 GetMaterialType() const;
    //-------------------------------------------



    /* ---   Data   --- */

    /** Установить и применить настройки по Данным клетки */
    void SetData(const FSquareData& Data);

    /** Получить данные (неизменяемые) */
    const FSquareData& GetData();

    /** Занять Клетку определённым типом стороны */
    void OccupySquare(const EWarringPartiesType& WarringPartiesType);

    /** Установить указатель на текущий Генератор клеток */
    void SetPointerToSquareGenerator(ASquareGenerator* SquareGenerator);
    //-------------------------------------------



private:

    /* ---   Generator   --- */

    // Данные Клетки
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    FSquareData SquareData;

    // Указатель на текущий Генератор клеток
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    ASquareGenerator* CurrentSquareGenerator = nullptr;

    //

    /** Установка материала по номеру */
    void UpdateMaterialByType(const int32& NumType);
    //-------------------------------------------
};
