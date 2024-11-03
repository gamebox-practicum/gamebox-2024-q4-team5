#include "Index2D.h"



/* ---   Constructors   --- */

FIndex2D::FIndex2D(const int32& iX, const int32& iY) : X(iX), Y(iY) {}
//--------------------------------------------------------------------------------------



/* ---   Operators   --- */

FIndex2D FIndex2D::operator-(const int32& Scale) const
{
    return FIndex2D(X - Scale, Y - Scale);
}
//--------------------------------------------------------------------------------------



/* ---   Others   --- */

bool FIndex2D::Within(const FIndex2D& V) const
{
    return X > V.X && Y > V.Y;
};
//--------------------------------------------------------------------------------------



/* ---   Operators Outside Class   --- */

FVector operator*(const FVector& iVector, const FIndex2D& iIndex)
{
    FVector lResult = iVector;
    lResult.X *= iIndex.X;
    lResult.Y *= iIndex.Y;

    return lResult;
}

FVector operator*(const FIndex2D& iIndex, const FVector& iVector)
{
    return iVector * iIndex;
}
//--------------------------------------------------------------------------------------
