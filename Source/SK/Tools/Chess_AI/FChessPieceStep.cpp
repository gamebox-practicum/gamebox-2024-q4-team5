#include "FChessPieceStep.h"

#include "UChessPieceInfo.h"

float FChessPieceStep::GetStepScore()
{
    return AttackedPiece == nullptr? 0.0f : AttackedPiece->GetRelativeValue();
}
