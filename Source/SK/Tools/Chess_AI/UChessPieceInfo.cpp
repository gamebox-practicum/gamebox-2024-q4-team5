#include "UChessPieceInfo.h"

#include <stdexcept>

std::unique_ptr<std::vector<FIndex2D>> UChessPieceInfo::GetLegalMoves(UChessBoardInfo* ChessBoardInfo)
{
    throw std::runtime_error("trying to get moves from an empty chess square");
}
