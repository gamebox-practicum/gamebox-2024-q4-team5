#pragma once

// Base:
#include "CoreMinimal.h"

// UE:
#include "Engine/DataTable.h"

// Tools:
#include "SK/Tools/Index2D.h"

// Structs:
#include "SK/ChessBoard/SquareComponent.h"
#include "SK/ChessMans/ChessManStruct.h"

// Generated:
#include "LevelData.generated.h"
//--------------------------------------------------------------------------------------



// Структура данных игры
USTRUCT(BlueprintType)
struct FLevelData
{
    GENERATED_BODY()

    /* ---   Chess Operator Data   --- */

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
    FIndex2D NumberAlongAxes = FIndex2D();

    // Массив данных Компонентов клеток
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSquareComponentData> SquareComponentsData;
    //-------------------------------------------


    /* ---   ChessMan Generator Data   --- */

    // Массив данных Шахматных фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FChessManData> ChessMansData;

    // Массив данных Игроков
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FPlayerData> PlayersData;
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
    0,       // Номер текущего этапа
    0,       // Время таймера в секундах
    nullptr, // Таблица данных конструирования уровня
    //-------------------------------------------

    /* ---   Square Generator Data   --- */
    FIndex2D(), // Количество клеток вдоль осей
    TArray<FSquareComponentData>(), // Массив данных Компонентов клеток
    //-------------------------------------------

    /* ---   Square Generator Data   --- */
    TArray<FChessManData>(), // Массив данных Шахматных фигур
    TArray<FPlayerData>(),   // Массив данных Игроков
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------