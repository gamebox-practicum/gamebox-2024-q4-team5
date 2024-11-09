// Fill out your copyright notice in the Description page of Project Settings.


#include "Queen.h"

int UQueen::GetRelativeValue()
{
    return 9;
}

FString UQueen::GetLogView()
{
    if(Color == PIECE_COLOR::WHITE)
    {
        return "Q";
    }
    return "q";
}

std::unique_ptr<std::vector<FChessPieceStep>> UQueen::GetLegalMoves(UChessBoardInfo* ChessBoardInfo)
{
    const FCellIndex directions[] = {{1,-1},{1,0},{1,1},
                                     {0,-1},                {0,1},
                                     {-1,-1}, {-1,0},{-1,1}};
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
