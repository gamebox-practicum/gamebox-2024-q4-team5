#pragma once

// Base:
#include "CoreMinimal.h"

// Tools:
#include "SK/Tools/Index2D.h"

// UE:
#include "Engine/DataTable.h"

// Generated:
#include "ChessManStruct.generated.h"
//--------------------------------------------------------------------------------------



// Тип Фигуры
UENUM(BlueprintType)
enum struct EChessManType : uint8
{
    // Пешка
    Pawn,
    // Конь
    Knight,
    // Слон
    Bishop,
    // Ладья
    Rook,
    // Ферзь (Королева)
    Queen,

    // Базовый без эффектов
    NONE	// Всегда в конце для строгого назначения количества элементов массива
};



// Данные Фигуры
USTRUCT(BlueprintType)
struct FChessManData : public FTableRowBase
{
    GENERATED_BODY()

    // Тип Фигуры
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EChessManType Type = EChessManType::NONE;

    // Позиция на доске
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIndex2D Position = { 0,0 };



    /* ---   Constructors   --- */
    FChessManData() {};

    FChessManData(EChessManType iType, FIndex2D iPosition)
        : Type(iType), Position(iPosition) {};
    //-------------------------------------------
};
