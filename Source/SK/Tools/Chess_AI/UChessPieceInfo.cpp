#include "UChessPieceInfo.h"
#include "FCellIndex.h"

std::unique_ptr<std::vector<FCellIndex>> UChessPieceInfo::GetLegalMoves(UChessBoardInfo* ChessBoardInfo)
{
    //UCLASS does not support abstract functions, so it is empty
    return nullptr;
}

int UChessPieceInfo::GetRelativeValue()
{
    //UCLASS does not support abstract functions, so it is empty
    return 0;
}
