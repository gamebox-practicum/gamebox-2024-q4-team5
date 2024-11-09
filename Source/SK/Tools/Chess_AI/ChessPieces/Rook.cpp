// Fill out your copyright notice in the Description page of Project Settings.


#include "Rook.h"

int URook::GetRelativeValue()
{
    return 5;
}

FString URook::GetLogView()
{
    if(Color == PIECE_COLOR::WHITE)
    {
        return "R";
    }
    return "r";
}

std::unique_ptr<std::vector<FChessPieceStep>> URook::GetLegalMoves(UChessBoardInfo* ChessBoardInfo)
{
    const FCellIndex directions[] = {              {1,0},
                                     {0,-1},                {0,1},
                                                   {-1,0},};
    auto result = std::make_unique<std::vector<FChessPieceStep>>();
    for(FCellIndex direction : directions)
    {
        auto target =  CurrentCell;
        do
        {
            target =  target + direction;
            PushStepIfValid(ChessBoardInfo, target, result.get());
        } while(CanJumpOver(ChessBoardInfo, target));
    }
    return result;
}
