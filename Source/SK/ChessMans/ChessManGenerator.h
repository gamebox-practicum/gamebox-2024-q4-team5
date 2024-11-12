// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "SK/ChessOperators/ChessGenerator.h"

// Tools:
#include "SK/Tools/Index2D.h"

// Structs:
#include "ChessManStruct.h"

// Generated:
#include "ChessManGenerator.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// UE:
class UDataTable;

// Interaction:
class ASquare;
class ASK_Character;
class AChessMan;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API AChessManGenerator : public AChessGenerator
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    AChessManGenerator();
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Called when the game starts or when spawned
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



    /* ---   Generator | Players   --- */

    // Таблица данных местоположения фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generator",
        meta = (RequiredAssetDataTags = "RowStructure=PlayerData"))
    UDataTable* PlayersTable;
    //-------------------------------------------



    /* ---   Generator | ChessMan   --- */

    // Таблица данных местоположения фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generator",
        meta = (RequiredAssetDataTags = "RowStructure=ChessManData"))
    UDataTable* ChessMansTable;

    // Класс Шахматной фигуры, соответствующий конкретному типу
    // 
    // PS: Массив НЕ динамический.
    // Количество элементов массива зависит от количества элементов в EChessManType
    UPROPERTY(EditAnywhere, Category = "Settings|Generator",
        meta = (ArraySizeEnum = "EChessManType"))
    TSubclassOf<AChessMan> ChessManType[EChessManType::NONE];
    //-------------------------------------------



    /* ---   Data   --- */

    /** Сохранить двумерный массив указателей на Клетки */
    void SetPointerToAllSquares(FSquareArray2D* Squares);
    // PS: UFUNCTION() для него не работает

    /** Получение указателя на все Фигуры Игроков */
    TArray<ASK_Character*>* GetPointerToAllPlayers();

    /** Получение указателя на все Шахматные фигуры */
    TArray<AChessMan*>* GetPointerToAllChessMans();

    /** Получение указателя на все доступные Шахматные фигуры */
    TArray<AChessMan*>* GetPointerToAllAvailableChessMans();

    /** Обновить данные о всех Шахматных фигурах */
    void UpdateAllChessMan();

    /** Обновить данные о доступных Шахматных фигурах */
    void UpdateAllAvailableChessMan();
    //-------------------------------------------



    /* ---   Stage   --- */

    /** Добавить генерируемых Шахматных фигур */
    void AddGeneratedChessMans(UDataTable* PlayersTable, UDataTable* ChessMansTable);
    //-------------------------------------------



private:

    /* ---   Re Generate   --- */

    // Тег для определения Шахматных фигур, созданной генератором
    FName VerificationTag = FName(GetNameSafe(this));

    //

    /** Удалить все Фигуры игроков */
    void DeleteAllPlayers();

    /** Удалить все Шахматные фигуры */
    void DeleteAllChessMans();

    template<class T>
    T* CreateFigureOnChessboard(const TSubclassOf<AActor>& Type, const FIndex2D& XY);
    //-------------------------------------------



    /* ---   Generator | Players   --- */

    /** Запуск генерации Фигур игроков */
    void CreateGeneratedPlayers(UDataTable* PlayersTable);
    //-------------------------------------------



    /* ---   Generator | ChessMan   --- */

    /** Запуск генерации Шахматных фигур */
    void CreateGeneratedChessMans(UDataTable* ChessMansTable);
    //-------------------------------------------



    /* ---   Data   --- */

    // Двумерный массив указателей на Клетки
    FSquareArray2D* PointerToAllSquares;

    // Все Фигуры Игроков
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    TArray<ASK_Character*> AllPlayers;

    // Все Шахматные фигуры
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    TArray<AChessMan*> AllChessMans;

    // Все доступные Шахматные фигуры (имеют вариант хода)
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    TArray<AChessMan*> AllAvailableChessMan;
    //-------------------------------------------
};
