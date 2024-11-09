// Fill out your copyright notice in the Description page of Project Settings.


#include "King.h"

#include "SK/Tools/Chess_AI/ChessBoardInfo.h"

int UKing::GetRelativeValue()
{
    return 999;
}

std::unique_ptr<std::vector<FChessPieceStep>> UKing::GetLegalMoves(UChessBoardInfo* ChessBoardInfo)
{
    const FCellIndex possibleSteps[] = {{1,-1},{1,0},{1,1},
                                         {0,-1},                {0,1},
                                         {-1,-1}, {-1,0},{-1,1}};
    auto result = std::make_unique<std::vector<FChessPieceStep>>();
    for(FCellIndex target : possibleSteps)
    {
        target =  target + CurrentCell;
        PushStepIfValid(ChessBoardInfo, target, result.get());
    }
    return result;
}
