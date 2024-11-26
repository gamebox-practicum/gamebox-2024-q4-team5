//

// Base:
#include "Index2D.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

FIndex2D::FIndex2D(const FIndex2D& Second)
{
    *this = Second;
}

FIndex2D::FIndex2D(const int32& iX, const int32& iY) : X(iX), Y(iY) {}
FIndex2D::FIndex2D(const int32& iScale) : FIndex2D(iScale, iScale) {}
//--------------------------------------------------------------------------------------



/* ---   Operators | =   --- */

const FIndex2D& FIndex2D::operator=(const FIndex2D& Second)
{
    X = Second.X;
    Y = Second.Y;

    return *this;
}

bool FIndex2D::operator==(const FIndex2D& Second)
{
    return X == Second.X && Y == Second.Y;
}

bool FIndex2D::operator!=(const FIndex2D& Second)
{
    return X != Second.X || Y != Second.Y;
}
//--------------------------------------------------------------------------------------



/* ---   Operators | ><   --- */

bool FIndex2D::operator>(const FIndex2D& Second)
{
    return X > Second.X && Y > Second.Y;
}

bool FIndex2D::operator>=(const FIndex2D& Second)
{
    return X >= Second.X && Y >= Second.Y;
}

bool FIndex2D::operator<(const FIndex2D& Second)
{
    return X < Second.X && Y < Second.Y;
}

bool FIndex2D::operator<=(const FIndex2D& Second)
{
    return X <= Second.X && Y <= Second.Y;
}
//--------------------------------------------------------------------------------------



/* ---   Operators | +   --- */

const FIndex2D FIndex2D::operator+(const FIndex2D& Second) const
{
    return FIndex2D(X + Second.X, Y + Second.Y);
}

FIndex2D& FIndex2D::operator+=(const FIndex2D& Second)
{
    *this = *this + Second;
    return *this;
}

FIndex2D& FIndex2D::operator++()
{
    ++X; ++Y;
    return *this;
}

FIndex2D FIndex2D::operator++(int32)
{
    FIndex2D tmp(*this);
    *this += 1;
    return tmp;
}
//--------------------------------------------------------------------------------------



/* ---   Operators | -   --- */

const FIndex2D FIndex2D::operator-(const FIndex2D& Second) const
{
    return FIndex2D(X - Second.X, Y - Second.Y);
}

FIndex2D& FIndex2D::operator-=(const FIndex2D& Second)
{
    *this = *this - Second;
    return *this;
}

FIndex2D& FIndex2D::operator--()
{
    --X; --Y;
    return *this;
}

FIndex2D FIndex2D::operator--(int32)
{
    FIndex2D tmp(*this);
    *this -= 1;
    return tmp;
}
//--------------------------------------------------------------------------------------



/* ---   Others   --- */

bool FIndex2D::Within(const FIndex2D& V) const
{
    return X > V.X && Y > V.Y;
}

FIndex2D FIndex2D::Distance(const FIndex2D& V) const
{
    return FIndex2D(abs(X - V.X), abs(Y - V.Y));
}
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
