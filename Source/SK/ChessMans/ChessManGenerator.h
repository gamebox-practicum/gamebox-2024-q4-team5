// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

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

    /** Перегенерировать (перезапустить) данный Генератор */
    UFUNCTION(BlueprintCallable, Category = "Settings", CallInEditor)
    void ReGenerate();
    //-------------------------------------------



    /* ---   Generator | Players   --- */

    // Таблица данных местоположения фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generator|ChessMan",
        meta = (RequiredAssetDataTags = "RowStructure=PlayerData"))
    UDataTable* PlayersTable;
    //-------------------------------------------



    /* ---   Generator | ChessMan   --- */

    // Таблица данных местоположения фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generator|ChessMan",
        meta = (RequiredAssetDataTags = "RowStructure=ChessManData"))
    UDataTable* ChessMansTable;

    // Класс Шахматной фигуры, соответствующий конкретному типу
    // 
    // PS: Массив НЕ динамический.
    // Количество элементов массива зависит от количества элементов в EChessManType
    UPROPERTY(EditAnywhere, Category = "Settings|Generator")
    TSubclassOf<AChessMan> SquareType[EChessManType::NONE];
    //-------------------------------------------



    /* ---   Data   --- */

    /** Сохранить двумерный массив указателей на Клетки */
    void SetPointerToAllSquares(const TArray<TArray<ASquare*>>* Squares);
    // PS: UFUNCTION() для него не работает

    /** Получение указателя на все Фигуры Игроков */
    TArray<ASK_Character*>* GetPointerToAllPlayers();

    /** Получение указателя на все Шахматные фигуры */
    TArray<AChessMan*>* GetPointerToAllChessMans();

    /** Получение указателя на все доступные Шахматные фигуры */
    TArray<AChessMan*>* GetPointerToAllAvailableChessMans();
    //-------------------------------------------



private:

    /* ---   Re Generate   --- */

    // Тег для определения Шахматных фигур, созданной генератором
    FName VerificationTag = "Generate";

    //

    /** Удалить все Фигуры игроков */
    void DeleteAllPlayers();

    /** Получить все созданные Фигуры игроков (проверка по Тегу) */
    TArray<ASK_Character*> GetAllPlayers();

    /** Удалить все Шахматные фигуры */
    void DeleteAllChessMans();

    /** Получить все созданные Шахматные фигуры (проверка по Тегу) */
    TArray<AChessMan*> GetAllChessMan();
    //-------------------------------------------



    /* ---   Generator | Players   --- */

    /** Запуск генерации Фигур игроков */
    void CreateGeneratedPlayers();

    /** Создать Шахматную фигуру в указанной позиции */
    void CreatePlayer(const TSubclassOf<ASK_Character> Type, const FIndex2D& XY);
    //-------------------------------------------



    /* ---   Generator | ChessMan   --- */

    /** Запуск генерации Шахматных фигур */
    void CreateGeneratedChessMans();

    /** Создать Шахматную фигуру в указанной позиции */
    void CreateChessMan(const EChessManType Type, const FIndex2D& XY);
    //-------------------------------------------



    /* ---   Data   --- */

    // Двумерный массив указателей на Клетки
    const TArray<TArray<ASquare*>>* PointerToAllSquares;

    // Все Фигуры Игроков
    TArray<ASK_Character*> AllPlayers;

    // Все Шахматные фигуры
    TArray<AChessMan*> AllChessMans;

    // Все доступные Шахматные фигуры (имеют вариант хода)
    TArray<AChessMan*> AllAvailableChessMan;

    //

    /** Обновить данные о доступных Шахматных фигурах */
    void UpdateAllAvailableChessMan();
    //-------------------------------------------
};
