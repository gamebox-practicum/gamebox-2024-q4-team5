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
class AChessMan;
class AChessOperator;
class ADealerHand;
class ASquare;
class ASK_Character;
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

    // Тег для определения Шахматных фигур, созданной генератором
    FName VerificationTag = FName(GetNameSafe(this));

    //

    /** Перегенерировать (перезапустить) данный Генератор */
    UFUNCTION(BlueprintCallable, Category = "Settings", CallInEditor)
    void ReGenerate();

    /** Удалить все Фигуры игроков */
    void DeleteAllPlayers();

    /** Удалить все Шахматные фигуры */
    void DeleteAllChessMans();
    //-------------------------------------------



    /* ---   Generator | Dealer Hand   --- */

    // Тип генерируемой Руки Дилера
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Generator")
    TSubclassOf<ADealerHand> DealerHandType;
    //-------------------------------------------



    /* ---   Generator | Players   --- */

    // Таблица данных местоположения фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generator",
        meta = (RequiredAssetDataTags = "RowStructure=PlayerData"))
    UDataTable* PlayersTable;

    //

    /** Запуск генерации Фигур игроков по данным из Массива */
    void CreateGeneratedPlayers(const TArray<FPlayerData*>& PlayersData);

    /** Получить первого игрока из списка */
    ASK_Character* GetFirstPlayer();
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

    //

    /** Запуск генерации Шахматных фигур по данным из Массива */
    void CreateGeneratedChessMans(const TArray<FChessManData*>& ChessMansData);
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

    /** Получение указателя на все Атакующие фигуры */
    TArray<FAttackingChessMansData>* GetPointerToAttackingChessMans();

    /** Удалить данные о Игроке */
    void RemovePlayer(ASK_Character* Player);

    /** Удалить данные о Шахматной фигуре */
    void RemoveChessMan(AChessMan* ChessMan);

    /** Обновить данные о доступных Шахматных фигурах */
    void UpdateAllAvailableChessMan();

    /** Установить указатель на текущий Оператор */
    void SetPointerToOperator(AChessOperator* CurrentOperator);
    //-------------------------------------------



    /* ---   Stage   --- */

    /** Добавить генерируемых Шахматных фигур */
    void AddGeneratedChessMans(UDataTable* PlayersTable, UDataTable* ChessMansTable);
    //-------------------------------------------



private:

    /* ---   Re Generate   --- */

    template<class T>
    T* CreateFigureOnChessboard(const TSubclassOf<AActor>& Type, const FIndex2D& XY);
    //-------------------------------------------



    /* ---   Generator | Dealer Hand   --- */

    // Указатель на текущую Руку Дилера
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    ADealerHand* CurrentDealerHand = nullptr;

    //

    /** Запуск генерации Шахматных фигур */
    void CreateGeneratedDealerHand();
    //-------------------------------------------



    /* ---   Generator | Players   --- */

    /** Запуск генерации Фигур игроков по данным из Таблицы */
    void CreateGeneratedPlayers(UDataTable* PlayersTable);
    //-------------------------------------------



    /* ---   Generator | ChessMan   --- */

    /** Запуск генерации Шахматных фигур по данным из Таблицы */
    void CreateGeneratedChessMans(UDataTable* ChessMansTable);
    //-------------------------------------------



    /* ---   Data   --- */

    // Указатель на Двумерный массив указателей на Клетки
    FSquareArray2D* PointerToAllSquares;

    // Указатель на оператора, что управляет им
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    AChessOperator* CurrentOperator = nullptr;

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

    // Все Атакующие фигуры
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    TArray<FAttackingChessMansData> AttackingChessMans;
    //-------------------------------------------
};
