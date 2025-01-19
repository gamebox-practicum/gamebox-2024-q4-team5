// 

#pragma once

// Base:
#include "CoreMinimal.h"

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
struct FSquareComponentData;
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
    //-------------------------------------------



public:

    /* ---   Base   --- */

    /** Вызывается при размещении экземпляра данного класса (в редакторе) или его запуске.
    @param	Transform	-- Трансформация данного Актора.
    */
    virtual void OnConstruction(const FTransform& Transform) override;
    //-------------------------------------------



    /* ---   Re Generate   --- */

    // Тег для определения клетки, созданной генератором
    FName VerificationTag = FName(GetNameSafe(this));

    //

    /** Перегенерировать (перезапустить) данный Генератор */
    UFUNCTION(BlueprintCallable, CallInEditor,
        Category = "Settings")
    void ReGenerate();

    /** Пересоздать все Клетки */
    void RecreateBoard();
    //-------------------------------------------



    /* ---   Generator   --- */

    // Тип генерируемого блока
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
        Category = "Settings|Generator")
    TSubclassOf<ASquare> SquareType;

    // Начинать расставлять доску с белой клетки?
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
        Category = "Settings|Generator",
        meta = (DisplayName = "White First?"))
    bool bWhiteSquareFirst = false;

    // Количество вдоль осей
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
        Category = "Settings|Generator")
    FIndex2D NumberAlongAxes = { 10, 10 };

    // Тип генерируемого Триггера новой стадии уровня
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
        Category = "Settings|Generator")
    TSubclassOf<AStageTrigger> StageTriggerType;

    //

    /** Запуск генерации Клеток */
    void CreateGeneratedSquares();
    //-------------------------------------------



    /* ---   Generator | SquareData   --- */

    // Индексы позиции всех "трупов"
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly,
        Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    TArray<FIndex2D> CorpsesPositionIndex;

    //

    /** Получение всех клеток в виде двумерного массива */
    FSquareArray2D* GetPointerToAllSquares();

    /** Установить указатель на текущий Оператор */
    void SetPointerToOperator(AChessOperator* CurrentOperator);
    //-------------------------------------------



    /* ---   Square Components   --- */

    // Таблица данных местоположения компонентов Клеток
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Settings|Square Components",
        meta = (RequiredAssetDataTags = "RowStructure=SquareComponentData"))
    UDataTable* SquareComponentTable = nullptr;

    //

    /** Запуск генерации Компонентов Клеток по данным из Массива */
    void CreateGeneratedSquareComponents(const TArray<FSquareComponentData*>& SquareComponentsData);
    //-------------------------------------------



    /* ---   Stage   --- */

    /** Добавить генерируемых Клеток */
    void AddGeneratedSquares(const int32& AddOnX, UDataTable* SquareComponentTable);
    //-------------------------------------------



private:

    /* ---   Re Generate   --- */

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
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly,
        Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    AChessOperator* CurrentOperator = nullptr;

    //

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
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly,
        Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    FSquareArray2D TDArraySquares;

    //

    /** Загрузить данные в указанный блок */
    void SetSquareData(ASquare* Block, FSquareData Data);

    /** Генерация данных Клетки */
    FSquareData SquareDataGeneration(const FIndex2D& XY);

    /** Генерация номера материала по координатам */
    bool GetMaterialNumber(const FIndex2D& XY);
    //-------------------------------------------



    /* ---   Square Components   --- */

    /** Запуск генерации Компонентов Клеток по данным из Таблицы */
    void CreateGeneratedSquareComponents(UDataTable* SquareComponentTable);
    //-------------------------------------------
};
