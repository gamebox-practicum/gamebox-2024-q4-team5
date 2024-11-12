// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "SK/ChessOperators/ChessGenerator.h"

// Tools:
#include "SK/Tools/Index2D.h"

// Structs:
#include "SquareStruct.h"

// Generated:
#include "SquareGenerator.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// UE:
class UDataTable;

// Interaction:
class AChessOperator;
class ASquare;
class AStageTrigger;
class USquareComponent;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API ASquareGenerator : public AChessGenerator
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
    //-------------------------------------------



public:

    /* ---   Base   --- */

    /** Вызывается при размещении экземпляра данного класса (в редакторе) или его запуске.
    @param	Transform	-- Трансформация данного Актора.
    */
    virtual void OnConstruction(const FTransform& Transform) override;
    //-------------------------------------------



    /* ---   Re Generate   --- */

    /** Перегенерировать (перезапустить) данный Генератор */
    UFUNCTION(BlueprintCallable, Category = "Settings", CallInEditor)
    void ReGenerate();
    //-------------------------------------------



    /* ---   Generator   --- */

    // Тип генерируемого блока
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Generator")
    TSubclassOf<ASquare> SquareType;

    // Зазор между блоками и высота построения (ось Z)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Generator")
    FVector Gap = { 0.f, 0.f, 0.f };

    // Количество вдоль осей
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Generator")
    FIndex2D NumberAlongAxes = { 10, 10 };

    // Тип генерируемого Триггера новой стадии уровня
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Generator")
    TSubclassOf<AStageTrigger> StageTriggerType;
    //-------------------------------------------



    /* ---   Get Data   --- */

    /** Получение всех клеток в виде двумерного массива */
    FSquareArray2D* GetPointerToAllSquares();

    /** Установить указатель на текущий Оператор */
    void SetPointerToOperator(AChessOperator* CurrentOperator);
    //-------------------------------------------



    /* ---   Square Components   --- */

    // Таблица данных местоположения компонентов Клеток
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Square Components",
        meta = (RequiredAssetDataTags = "RowStructure=SquareComponentData"))
    UDataTable* SquareComponentTable = nullptr;
    //-------------------------------------------



    /* ---   Stage   --- */

    /** Добавить генерируемых Клеток */
    void AddGeneratedSquares(const int32& AddOnX, UDataTable* SquareComponentTable);
    //-------------------------------------------



private:

    /* ---   Re Generate   --- */

    // Тег для определения клетки, созданной генератором
    FName VerificationTag = FName(GetNameSafe(this));

    //

    /** Удалить все клетки */
    void DeleteAllSquares();

    /** Удалить все Компоненты Клеток */
    void DeleteAllSquareComponents();

    /** Удалить все Триггеры смены стадии уровня */
    void DeleteStageTriggers();
    //-------------------------------------------



    /* ---   Generator   --- */

    // Вычисленный размер блока
    FVector BlockSize = FVector::ZeroVector;

    // Вычисленное смещение точки
    FVector PointOffset = FVector::ZeroVector;

    // Указатель на оператора, что управляет им
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    AChessOperator* CurrentOperator = nullptr;

    //

    /** Запуск генерации Клеток */
    void CreateGeneratedSquares();

    /** Создать блок в указанной позиции */
    ASquare* CreateSquare(const FIndex2D& XY);

    /** Проверить данные, при необходимости обновляет их по Актору */
    void GetSquareSize(const ASquare* Block);

    /** Получить локацию блока с указанным индексом матрицы */
    FVector GetLocationForSquare(const FIndex2D& XY) const;

    /** Сгенерировать Триггер смены стадии уровня */
    void CreatStageTrigger();

    /** Рассчитать и получить локацию для генерируемого Триггера смены стадии уровня */
    FVector GetLocationForStageTrigger();

    /** Рассчитать и получить размеры для генерируемого Триггера смены стадии уровня */
    FVector GetScaleForStageTrigger();
    //-------------------------------------------



    /* ---   Generator | SquareData   --- */

    // Все клетки в виде двумерного массива
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    FSquareArray2D TDArraySquares;

    //

    /** Загрузить данные в указанный блок */
    void SetSquareData(ASquare* Block, FSquareData Data);

    /** Генерация данных Клетки */
    FSquareData SquareDataGeneration(const FIndex2D& XY);

    /** Генерация номера материала по координатам */
    int32 GetMaterialNumber(const FIndex2D& XY);
    //-------------------------------------------



    /* ---   Square Components   --- */

    /** Запуск генерации Компонентов Клеток */
    void CreateGeneratedSquareComponents(UDataTable* SquareComponentTable);
    //-------------------------------------------
};
