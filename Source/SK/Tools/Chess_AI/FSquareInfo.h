#pragma once

class UChessPieceInfo;
struct FSquareInfo
{
public:
    UChessPieceInfo* CurrentPiece = nullptr;

    bool IsCanStepOn = true;
};
