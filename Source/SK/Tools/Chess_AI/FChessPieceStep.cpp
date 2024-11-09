#include "FChessPieceStep.h"

#include "UChessPieceInfo.h"

int FChessPieceStep::GetStepScore()
{
    return AttackedPiece == nullptr? 0 : AttackedPiece->GetRelativeValue();
}
