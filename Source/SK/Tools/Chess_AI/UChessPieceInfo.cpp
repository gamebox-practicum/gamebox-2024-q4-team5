#include "UChessPieceInfo.h"

#include "ChessBoardInfo.h"
#include "FCellIndex.h"
#include "FChessPieceStep.h"

std::unique_ptr<std::vector<FChessPieceStep>> UChessPieceInfo::GetLegalMoves(UChessBoardInfo* ChessBoardInfo)
{
    //UCLASS does not support abstract functions, so it is empty
    return nullptr;
}

int UChessPieceInfo::GetRelativeValue()
{
    //UCLASS does not support abstract functions, so it is empty
    return 0;
}

void UChessPieceInfo::PushStepIfValid(UChessBoardInfo* ChessBoardInfo, FCellIndex Target,
    std::vector<FChessPieceStep>* Result)
{
    if(ChessBoardInfo->IsValidCell(Target))
    {
        if(UChessPieceInfo* targetPiece = (*ChessBoardInfo)[Target].CurrentPiece)
        {
            if(targetPiece->Color != this->Color)
            {
                Result->push_back({this->CurrentCell, Target, targetPiece});
            }
        }
        else
        {
            Result->push_back({this->CurrentCell, Target, nullptr});
        }
    }
}
