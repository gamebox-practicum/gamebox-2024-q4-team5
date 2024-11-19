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

    float GetStepScore();
};

inline bool operator== (const FChessPieceStep &step1, const FChessPieceStep &step2)
{
    return (step1.PreviousPosition == step2.PreviousPosition) &&
            (step1.NewPosition == step2.NewPosition) &&
            (step1.AttackedPiece == step2.AttackedPiece);
}

inline bool operator!= (const FChessPieceStep &step1, const FChessPieceStep &step2)
{
    return !(step1 == step2);
}

static const FChessPieceStep UNABLE_MOVE {{0, 0},{0, 0}, nullptr};
