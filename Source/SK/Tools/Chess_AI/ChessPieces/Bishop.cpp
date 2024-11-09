// Fill out your copyright notice in the Description page of Project Settings.


#include "Bishop.h"

int UBishop::GetRelativeValue()
{
    return 3;
}

FString UBishop::GetLogView()
{
    if(Color == PIECE_COLOR::WHITE)
    {
        return "B";
    }
    return "b";
}

std::unique_ptr<std::vector<FChessPieceStep>> UBishop::GetLegalMoves(UChessBoardInfo* ChessBoardInfo)
{
    const FCellIndex directions[] = {{1,-1},                {1,1},

                                     {-1,-1},               {-1,1}};
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
