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
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Square Generator")
    FIndex2D NumberAlongAxes = { 10, 10 };
    //-------------------------------------------



    /* ---   Generators | ChessMan Generator   --- */

    // Таблица данных местоположения фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|ChessMan Generator",
        meta = (RequiredAssetDataTags = "RowStructure=PlayerData"))
    UDataTable* PlayersTable;

    // Таблица данных местоположения фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|ChessMan Generator",
        meta = (RequiredAssetDataTags = "RowStructure=ChessManData"))
    UDataTable* ChessMansTable;
    //-------------------------------------------



private:

    /* ---   Generators   --- */

    /** Template: Получить первый Актор конкретного типа*/
    template<typename T>
    T* GetFirstActor();
    //-------------------------------------------



    /* ---   Generators | Square Generator   --- */

    // Указатель на текущий Генератор Клеток
    ASquareGenerator* CurrentSquareGenerator;

    //

    /** Проверка указателя на текущий Генератор Клеток */
    void CheckCurrentSquareGenerator();

    /** Получить первый Генератор Клеток */
    ASquareGenerator* GetFirstSquareGenerator();

    /** Обновить данные связанные с Генератором Клеток */
    void UpdateSquareGeneratorData();
    //-------------------------------------------



    /* ---   Generators | ChessMan Generator   --- */

    // Указатель на текущий Генератор Шахматных фигур
    AChessManGenerator* CurrentChessManGenerator;

    //

    /** Проверка указателя на текущий Генератор Шахматных фигур */
    void CheckCurrentChessManGenerator();

    /** Получить первый Генератор Шахматных фигур */
    AChessManGenerator* GetFirstChessManGenerator();

    /** Обновить данные связанные с Генератором Шахматных фигур */
    void UpdateChessManGeneratorData();
    //-------------------------------------------
};
