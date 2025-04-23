// 

#pragma once

// Base:
#include "CoreMinimal.h"

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
class SK_API AChessManGenerator : public AActor
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
    FORCEINLINE void CreateGeneratedChessMans(const TArray<FChessManData*>& ChessMansData)
    {
        // Создать Шахматную фигуру согласно данным
        for (auto& lData : ChessMansData)
        {
            CreateChessMansFromData(*lData);
        }
    };

    /** Запуск генерации Шахматных фигур по данным */
    void CreateChessMansFromData(const FChessManData& Data);
    //-------------------------------------------



    /* ---   Data   --- */

    /** Сохранить двумерный массив указателей на Клетки */
    FORCEINLINE void SetPointerToAllSquares(FSquareArray2D* Squares)
    {
        PointerToAllSquares = Squares;
    };
    // PS: UFUNCTION() для него не работает

    /** Получение указателя на все Фигуры Игроков */
    FORCEINLINE TArray<ASK_Character*>* GetPointerToAllPlayers()
    {
        return &AllPlayers;
    };

    /** Получение указателя на все Шахматные фигуры */
    FORCEINLINE TArray<AChessMan*>* GetPointerToAllChessMans()
    {
        return &AllChessMans;
    };

    /** Получение указателя на все доступные Шахматные фигуры */
    FORCEINLINE TArray<AChessMan*>* GetPointerToAllAvailableChessMans()
    {
        return &AllAvailableChessMan;
    };

    /** Получение указателя на все Атакующие фигуры */
    FORCEINLINE TArray<FAttackingChessMansData>* GetPointerToAttackingChessMans()
    {
        return &AttackingChessMans;
    };

    /** Удалить данные о Игроке */
    FORCEINLINE void RemovePlayer(ASK_Character* Player)
    {
        AllPlayers.Remove(Player);
    };

    /** Удалить данные о Шахматной фигуре */
    FORCEINLINE void RemoveChessMan(AChessMan* ChessMan)
    {
        AllAvailableChessMan.RemoveSwap(ChessMan);
        AllChessMans.RemoveSwap(ChessMan);
    };

    /** Обновить данные о доступных Шахматных фигурах */
    void UpdateAllAvailableChessMan();

    /** Установить указатель на текущий Оператор */
    FORCEINLINE void SetPointerToOperator(AChessOperator* Operator)
    {
        CurrentOperator = Operator;
    };
    //-------------------------------------------



    /* ---   Stage   --- */

    /** Добавить генерируемых Шахматных фигур */
    FORCEINLINE void AddGeneratedChessMans(UDataTable* iPlayersTable, UDataTable* iChessMansTable)
    {
        CreateGeneratedPlayers(iPlayersTable);
        CreateGeneratedChessMans(iChessMansTable);
    };
    //-------------------------------------------



private:

    /* ---   Re Generate   --- */

    template<class T>
    T* CreateFigureOnChessboard(const TSubclassOf<AActor>& Type, const FIntPoint& XY);
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
