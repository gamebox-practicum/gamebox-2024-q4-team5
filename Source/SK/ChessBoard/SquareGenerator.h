// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Tools:
//#include "SquareData.h"
#include "SK/Tools/MyRandom.h"
#include "SK/Tools/Index2D.h"

// Generated:
#include "SquareGenerator.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// Interaction:
class ASquare;
struct FSquareData;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API ASquareGenerator : public AActor
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    ASquareGenerator();
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    /** Overridable native event for when play begins for this actor */
    virtual void BeginPlay() override;

    /** Overridable function called whenever this actor is being removed from a level */
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    //-------------------------------------------



public:

    /* ---   Base   --- */

    /** Вызывается при размещении экземпляра данного класса (в редакторе) или его запуске.
    @param	Transform	-- Трансформация данного Актора.
    */
    virtual void OnConstruction(const FTransform& Transform) override;
    //-------------------------------------------


    /* ---   Generator   --- */

    // Тип генерируемого блока
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
    TSubclassOf<ASquare> BlockType;

    // Зазор между блоками
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
    FVector Gap = { 0.f, 0.f, 0.f };

    // Количество вдоль осей
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
    FIndex2D NumberAlongAxes = { 10, 10 };
    //-------------------------------------------



private:

    /* ---   Preview   --- */

    // Тег для определения клетки, созданной генератором
    FName VerificationTag = "Generate";

    //

    /** Предварительная визуализация */
    void Preview();

    /** Удалить все клетки */
    void DeleteAllSquares();

    /** Получить все созданные клетки (проверка по Тегу) */
    TArray<ASquare*> GetAllSquares();
    //-------------------------------------------



    /* ---   Generator   --- */

    // Вычисленный размер блока
    FVector BlockSize = FVector::ZeroVector;

    // Вычисленное смещение точки
    FVector PointOffset = FVector::ZeroVector;

    //

    /** Запуск генерации блоков */
    void CreateGeneratedSquares();

    /** Создать блок в указанной позиции */
    void CreateSquare(const FIndex2D& XY);

    /** Проверить данные, при необходимости обновляет их по Актору */
    void GetSquareSize(const ASquare* Block);

    /** Получить локацию блока с указанным индексом матрицы */
    FVector GetLocationForSquare(const FIndex2D& XY) const;
    //-------------------------------------------



    /* ---   Generator : SquareData   --- */

    /** Загрузить данные в указанный блок */
    void SetSquareData(ASquare* Block, FSquareData Data);

    /** Генерация данных Клетки */
    FSquareData SquareDataGeneration(const FIndex2D& XY);

    /** Генерация номера материала по координатам */
    int32 GetMaterialNumber(const FIndex2D& XY);
    //-------------------------------------------
};
