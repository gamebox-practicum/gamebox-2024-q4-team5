#pragma once
#include "FCellIndex.h"

class UChessPieceInfo;

#include "FChessPieceStep.generated.h"

USTRUCT(BlueprintType)
struct FChessPieceStep
{
    GENERATED_BODY()
public:
    FCellIndex PreviousPosition;
    FCellIndex NewPosition;

    UChessPieceInfo* AttackedPiece = nullptr;

};

static const FChessPieceStep UNABLE_MOVE {{0, 0},{0, 0}, nullptr};
