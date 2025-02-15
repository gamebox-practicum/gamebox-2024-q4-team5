#pragma once

// Base:
#include "CoreMinimal.h"

// UE:
#include "Engine/DataTable.h"

// Structs:
#include "SK/ChessBoard/SquareComponent.h"
#include "SK/ChessMans/ChessManStruct.h"

// Generated:
#include "LevelData.generated.h"
//--------------------------------------------------------------------------------------



// Структура данных Уровня
USTRUCT(BlueprintType)
struct FLevelData
{
    GENERATED_BODY()

    /* ---   Chess Operator Data   --- */

    // Флаг отслеживания хода игрока
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPlayersMove = true;

    // Номер текущего этапа
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentStageNum = 0;

    // Время таймера в секундах
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoveLimitTime = 0.f;

    // Таблица данных конструирования уровня
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        meta = (RequiredAssetDataTags = "RowStructure=ChessOperatorData"))
    UDataTable* OperatorTable = nullptr;
    //-------------------------------------------


    /* ---   Square Generator Data   --- */

    // Количество клеток вдоль осей
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIntPoint NumberAlongAxes = FIntPoint();

    // Массив данных Компонентов клеток
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSquareComponentData> SquareComponentsData;
    //-------------------------------------------


    /* ---   ChessMan Generator Data   --- */

    // Массив данных Игроков
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FPlayerData> PlayersData;

    // Массив данных Шахматных фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FChessManData> ChessMansData;
    //-------------------------------------------


    /* ---   Static   --- */

    // Пустой: Все значения структуры равны базовыми значениям
    static const FLevelData Empty;
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------



/* ---   Переопределение для статических значений структуры   --- */

// Пустой: Все значения структуры = 0
__declspec(selectany) const FLevelData FLevelData::Empty
{
    /* ---   Chess Operator Data   --- */
    true,    // Флаг отслеживания хода игрока
    0,       // Номер текущего этапа
    0,       // Время таймера в секундах
    nullptr, // Таблица данных конструирования уровня
    //-------------------------------------------

    /* ---   Square Generator Data   --- */
    FIntPoint(), // Количество клеток вдоль осей
    TArray<FSquareComponentData>(), // Массив данных Компонентов клеток
    //-------------------------------------------

    /* ---   Square Generator Data   --- */
    TArray<FPlayerData>(),   // Массив данных Игроков
    TArray<FChessManData>(), // Массив данных Шахматных фигур
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
