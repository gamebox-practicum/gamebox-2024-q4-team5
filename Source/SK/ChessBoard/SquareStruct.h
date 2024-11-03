#pragma once

// Base:
#include "CoreMinimal.h"

// Generated:
#include "SquareStruct.generated.h"
//--------------------------------------------------------------------------------------



// Данные Клетки
USTRUCT(BlueprintType)
struct FSquareData
{
    GENERATED_BODY()

    FSquareData() {};

    // Тип материала блока (номер по порядку: A=0, B=1)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TypeBlockMaterial = 0;
};
