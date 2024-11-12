//

// Base:
#include "SquareStruct.h"
//--------------------------------------------------------------------------------------



/* ---   Functions   --- */

/* ---   Functions | IsValidIndex   --- */

bool FSquareArray2D::IsValidIndex(const int32& iX, const int32& iY) const
{
    return Square2DArray.IsValidIndex(iX) && Square2DArray[iX].SquareArray.IsValidIndex(iY);
}

bool FSquareArray2D::IsValidIndex(const FIndex2D& iXY) const
{
    return IsValidIndex(iXY.X, iXY.Y);
}
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

bool FSquareArray2D::IsEmpty() const
{
    return IsValidIndex(0, 0);
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

void FSquareArray2D::SetNum(const FIndex2D& iXY)
{
    SetNum(iXY.X, iXY.Y);
}

FIndex2D FSquareArray2D::Num() const
{
    FIndex2D lResult = FIndex2D(Square2DArray.Num(), 0);

    if (Square2DArray.IsValidIndex(0))
    {
        lResult += FIndex2D(0, Square2DArray[0].SquareArray.Num());
    }

    return lResult;
}
//--------------------------------------------------------------------------------------



/* ---   Functions | Get/Set   --- */

TArray<ASquare*> FSquareArray2D::GetArrayByX(const int32& iX) const
{
    return Square2DArray[iX].SquareArray;
}

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

ASquare* FSquareArray2D::GetByIndex(const FIndex2D& iXY) const
{
    return GetByIndex(iXY.X, iXY.Y);
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
bool FSquareArray2D::SetByIndex(ASquare* iSquare, const FIndex2D& iXY)
{
    return SetByIndex(iSquare, iXY.X, iXY.Y);
}
//--------------------------------------------------------------------------------------
