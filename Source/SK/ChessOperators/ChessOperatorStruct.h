#pragma once

// Base:
#include "CoreMinimal.h"

// UE:
#include "Engine/DataTable.h"

// Generated:
#include "ChessOperatorStruct.generated.h"
//--------------------------------------------------------------------------------------



// Данные Оператора для генерации
USTRUCT(BlueprintType)
struct FChessOperatorData : public FTableRowBase
{
    GENERATED_BODY()

    // Количество добавляемых строк по оси X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AddOnX = 0;

    // Таблица данных местоположения Компонентов Клетки
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        meta = (RequiredAssetDataTags = "RowStructure=SquareComponentData"))
    UDataTable* SquareComponentTable = nullptr;

    // Таблица данных местоположения Игроков
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        meta = (RequiredAssetDataTags = "RowStructure=PlayerData"))
    UDataTable* PlayersTable = nullptr;

    // Таблица данных местоположения Шахматных фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        meta = (RequiredAssetDataTags = "RowStructure=ChessManData"))
    UDataTable* ChessMansTable = nullptr;
    //-------------------------------------------



    /* ---   Constructors   --- */

    FChessOperatorData() {};
    //-------------------------------------------
};
