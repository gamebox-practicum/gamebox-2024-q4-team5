//

// Base:
#include "SquareStruct.h"
//--------------------------------------------------------------------------------------



/* ---   Functions   --- */

/* ---   Functions | IsValidIndex   --- */

//--------------------------------------------------------------------------------------



/* ---   Functions | Empty   --- */

void FSquareArray2D::Empty()
{
    for (auto& lData : Square2DArray)
    {
        lData.SquareArray.Empty();
    }

    Square2DArray.Empty();
}
//--------------------------------------------------------------------------------------



/* ---   Functions | SetNum   --- */

void FSquareArray2D::SetNum(const int32& iX, const int32& iY)
{
    Square2DArray.SetNum(iX);

    for (auto& lData : Square2DArray)
    {
        lData.SquareArray.SetNum(iY);
    }
}

FIntPoint FSquareArray2D::Num() const
{
    FIntPoint lResult = FIntPoint(Square2DArray.Num(), 0);

    if (Square2DArray.IsValidIndex(0))
    {
        lResult += FIntPoint(0, Square2DArray[0].SquareArray.Num());
    }

    return lResult;
}
//--------------------------------------------------------------------------------------



/* ---   Functions | Get/Set   --- */

TArray<ASquare*> FSquareArray2D::GetArrayByY(const int32& iY) const
{
    TArray<ASquare*> lResult;
    lResult.SetNum(Square2DArray.Num());

    for (int32 i = 0; i < Square2DArray.Num(); ++i)
    {
        lResult[i] = Square2DArray[i].SquareArray[iY];
    }

    return lResult;
}

ASquare* FSquareArray2D::GetByIndex(const int32& iX, const int32& iY) const
{
    if (IsValidIndex(iX, iY))
        return Square2DArray[iX].SquareArray[iY];
    else
        return nullptr;
}

bool FSquareArray2D::SetByIndex(ASquare* iSquare, const int32& iX, const int32& iY)
{
    if (IsValidIndex(iX, iY))
    {
        Square2DArray[iX].SquareArray[iY] = iSquare;
        return true;
    }

    return false;
}
//--------------------------------------------------------------------------------------
