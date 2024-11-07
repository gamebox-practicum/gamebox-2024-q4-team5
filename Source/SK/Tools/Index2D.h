#pragma once

// Base:
#include "CoreMinimal.h"

// Generated:
#include "Index2D.generated.h"
//--------------------------------------------------------------------------------------



/** Индекс блоков 2D-матрицы */
USTRUCT(BlueprintType)
struct FIndex2D
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
    int32 X = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
    int32 Y = 0;



    /* ---   Constructors   --- */

    FIndex2D() {};

    FIndex2D(const FIndex2D& Second);

    FIndex2D(const int32& iX, const int32& iY);
    FIndex2D(const int32& iScale);
    //-------------------------------------------


    /* ---   Operators | =   --- */

    const FIndex2D& operator=(const FIndex2D& other);
    //-------------------------------------------



    /* ---   Operators | +   --- */

    const FIndex2D operator+(const FIndex2D& Second) const;
    FIndex2D& operator+=(const FIndex2D& Second);
    FIndex2D& operator++(); // prefix
    FIndex2D operator++(int32); // postfix
    //-------------------------------------------



    /* ---   Operators | -   --- */

    const FIndex2D operator-(const FIndex2D& Second) const;
    FIndex2D& operator-=(const FIndex2D& Second);
    FIndex2D& operator--(); // prefix
    FIndex2D operator--(int32); // postfix
    //-------------------------------------------



    /* ---   Others   --- */

    // Проверка "Внутри" ли указанная позиция
    // (не выходит ли за количество)
    bool Within(const FIndex2D& V) const;
    //-------------------------------------------
};



/* ---   Operators Outside Class   --- */

FVector operator*(const FVector& iVector, const FIndex2D& iIndex);
FVector operator*(const FIndex2D& iIndex, const FVector& iVector);
//--------------------------------------------------------------------------------------

//UE_LOG(LogTemp, Error, TEXT("'%s': ** is NOT"), *GetNameSafe(this));
