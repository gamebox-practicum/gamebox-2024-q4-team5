#pragma once

struct FCellIndex
{
public:
    int Y;
    int X;
};

inline bool operator== (const FCellIndex &inx1, const FCellIndex &inx2)
{
    return (inx1.X == inx2.X) &&
        (inx1.Y == inx2.Y);
}

inline FCellIndex operator+ (const FCellIndex &inx1, const FCellIndex &inx2)
{
    return FCellIndex{inx1.Y + inx2.Y, inx1.X + inx2.X};
}
