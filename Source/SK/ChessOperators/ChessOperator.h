// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Tools:
#include "SK/Tools/Index2D.h"

// Generated:
#include "ChessOperator.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Delegate   --- */

// Делегат хода Игроков
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayersMove, bool, bIsPlayersMove);
// ----------------------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// UE:
class UDataTable;

// Interaction:
class ASquareGenerator;
class AChessManGenerator;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API AChessOperator : public AActor
{
    GENERATED_BODY()

public:

    /* ---   Delegate   --- */
    FOnPlayersMove OnPlayersMove; // Делегат хода Игроков
    // ----------------------------------------------------------------------------------------------------



    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    AChessOperator();
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



    /* ---   Generators | Square Generator   --- */

    // Количество клеток доски вдоль осей
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generators|Square Generator")
    FIndex2D NumberAlongAxes = { 10, 10 };
    //-------------------------------------------



    /* ---   Generators | ChessMan Generator   --- */

    // Таблица данных местоположения фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generators|ChessMan Generator",
        meta = (RequiredAssetDataTags = "RowStructure=PlayerData"))
    UDataTable* PlayersTable;

    // Таблица данных местоположения фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generators|ChessMan Generator",
        meta = (RequiredAssetDataTags = "RowStructure=ChessManData"))
    UDataTable* ChessMansTable;
    //-------------------------------------------



    /* ---   Delegate   --- */

    UFUNCTION()
    void PlayersMove(bool bIsPlayersMove);
    //-------------------------------------------



private:

    /* ---   Generators   --- */

    /** Template: Получить первый Актор конкретного типа*/
    template<typename T>
    T* GetFirstActor();
    //-------------------------------------------



    /* ---   Generators | Square Generator   --- */

    // Указатель на текущий Генератор Клеток
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generators|Check",
        meta = (AllowPrivateAccess = true))
    ASquareGenerator* CurrentSquareGenerator;

    //

    /** Проверка указателя на текущий Генератор Клеток */
    void UpdateCurrentSquareGenerator();

    /** Проверка и получение указателя на текущий Генератор Клеток */
    ASquareGenerator* GetCurrentSquareGenerator();

    /** Получить первый (по порядку на карте) Генератор Клеток */
    ASquareGenerator* GetFirstSquareGenerator();
    //-------------------------------------------



    /* ---   Generators | ChessMan Generator   --- */

    // Указатель на текущий Генератор Шахматных фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generators|Check",
        meta = (AllowPrivateAccess = true))
    AChessManGenerator* CurrentChessManGenerator;

    //

    /** Обновить данные текущего Генератора Шахматных фигур */
    void UpdateCurrentChessManGenerator();

    /** Проверка и получение указателя на текущий Генератор Шахматных фигур */
    AChessManGenerator* GetCurrentChessManGenerator();

    /** Получить первый (по порядку на карте) Генератор Шахматных фигур */
    AChessManGenerator* GetFirstChessManGenerator();
    //-------------------------------------------
};
