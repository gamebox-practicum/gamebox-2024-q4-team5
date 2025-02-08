#pragma once

// Base:
#include "CoreMinimal.h"

// UE:
#include "Engine/DataTable.h"

// Generated:
#include "LevelSelectionData.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Table Row   --- */

// Данные Уровня для запуска
USTRUCT(BlueprintType)
struct FLevelTableRow : public FTableRowBase
{
    GENERATED_BODY()

    // Запускаемая Карта уровня
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UWorld> Map;

    // Смещение позиции Игрока по оси Y, относительно предыдущей Карты уровня
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OffsetPlayersPositionAlongY = 0;
    //-------------------------------------------


    /* ---   Constructors   --- */

    FLevelTableRow() {};
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------



/* ---   Data   --- */

// Структура данных о выборе уровня
USTRUCT(BlueprintType)
struct FLevelSelectionData
{
    GENERATED_BODY()


    /* ---   Selection   --- */

    // Таблица данных порядка прохождения уровней
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        meta = (RequiredAssetDataTags = "RowStructure=LevelTableRow"))
    UDataTable* LevelsTable = nullptr;

    // Номер текущего этапа
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 LevelNumber = 0;
    //-------------------------------------------


    /* ---   Static   --- */

    // Пустой: Все значения структуры равны базовыми значениям
    static const FLevelSelectionData Empty;
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------



/* ---   Переопределение для статических значений структуры   --- */

// Пустой: Все значения структуры = 0
__declspec(selectany) const FLevelSelectionData FLevelSelectionData::Empty
{
    /* ---   Selection   --- */
    nullptr, // Таблица данных порядка прохождения уровней
    -1,       // Номер текущего этапа
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
