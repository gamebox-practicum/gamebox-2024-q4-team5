//

// Base:
#include "SquareStruct.h"
//--------------------------------------------------------------------------------------



/* ---   Functions   --- */

/* ---   Functions | IsValidIndex   --- */

bool FSquareArray2D::IsValidIndex(const int32& iX, const int32& iY) const
{
    return SquareArray.IsValidIndex(iX) && SquareArray[iX].SquareArray.IsValidIndex(iY);
}

bool FSquareArray2D::IsValidIndex(const FIndex2D& iXY) const
{
    return IsValidIndex(iXY.X, iXY.Y);
}
//--------------------------------------------------------------------------------------



/* ---   Functions | Empty   --- */

void FSquareArray2D::Empty()
{
    for (auto& lData : SquareArray)
    {
        lData.SquareArray.Empty();
    }

    SquareArray.Empty();
}

bool FSquareArray2D::IsEmpty() const
{
    return IsValidIndex(0, 0);
}
//--------------------------------------------------------------------------------------



/* ---   Functions | SetNum   --- */

void FSquareArray2D::SetNum(const int32& iX, const int32& iY)
{
    SquareArray.SetNum(iX);

    for (auto& lData : SquareArray)
    {
        lData.SquareArray.SetNum(iY);
    }
}

void FSquareArray2D::SetNum(const FIndex2D& iXY)
{
    SetNum(iXY.X, iXY.Y);
}
//--------------------------------------------------------------------------------------



/* ---   Functions | Get/Set   --- */

TArray<ASquare*> FSquareArray2D::GetArrayByX(const int32& iX) const
{
    return SquareArray[iX].SquareArray;
}

TArray<ASquare*> FSquareArray2D::GetArrayByY(const int32& iY) const
{
    TArray<ASquare*> lResult;
    lResult.SetNum(SquareArray.Num());

    for (int32 i = 0; i < SquareArray.Num(); ++i)
    {
        lResult[i] = SquareArray[i].SquareArray[iY];
    }

    return lResult;
}

ASquare* FSquareArray2D::GetByIndex(const int32& iX, const int32& iY) const
{
    return SquareArray[iX].SquareArray[iY];
}

ASquare* FSquareArray2D::GetByIndex(const FIndex2D& iXY) const
{
    return GetByIndex(iXY.X, iXY.Y);
}

bool FSquareArray2D::SetByIndex(ASquare* iSquare, const int32& iX, const int32& iY)
{
    if (IsValidIndex(iX, iY))
    {
        SquareArray[iX].SquareArray[iY] = iSquare;
        return true;
    }

    return false;
}
bool FSquareArray2D::SetByIndex(ASquare* iSquare, const FIndex2D& iXY)
{
    return SetByIndex(iSquare, iXY.X, iXY.Y);
}
//--------------------------------------------------------------------------------------
