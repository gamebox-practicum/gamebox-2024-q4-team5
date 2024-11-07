#pragma once

// Base:
#include "CoreMinimal.h"

// Tools:
#include "SK/Tools/Index2D.h"

// Generated:
#include "SquareStruct.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// Interaction:
class ASquare;
//--------------------------------------------------------------------------------------



// Тип враждующих сторон
UENUM(BlueprintType)
enum struct EWarringPartiesType : uint8
{
    // Свободный
    NONE = 0,
    // Белый
    White = 1,
    // Чёрный
    Black,
    // Труп Шахматной фигуры
    Corpse,
};



// Данные Клетки
USTRUCT(BlueprintType)
struct FSquareData
{
    GENERATED_BODY()

    // Позиционный номер
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIndex2D PositionNumber = { 0,0 };

    // Тип материала блока (номер по порядку: A=0, B=1)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TypeBlockMaterial = 0;

    // Тип стороны конфликта
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWarringPartiesType WarringPartiesType = EWarringPartiesType::NONE;
    //-------------------------------------------



    /* ---   Constructors   --- */

    FSquareData() {};
    //-------------------------------------------
};



// Массив указателей на Клетки
USTRUCT(BlueprintType)
struct FSquareArray
{
    // PS: Необходим для создания двумерного массива.
    // Не получилось преобразовать в шаблон, который бы поддерживался в UE

    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        meta = (DisplayName = "Y"))
    TArray<ASquare*> SquareArray;
    //-------------------------------------------



    /* ---   Constructors   --- */

    FSquareArray() {};
    //-------------------------------------------
};
