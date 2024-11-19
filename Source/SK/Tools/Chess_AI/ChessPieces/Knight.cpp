// Fill out your copyright notice in the Description page of Project Settings.


#include "Knight.h"

float UKnight::GetRelativeValue()
{
    return 3;
}

std::unique_ptr<std::vector<FChessPieceStep>> UKnight::GetLegalMoves(UChessBoardInfo* ChessBoardInfo)
{
    const FCellIndex possibleSteps[] = {{2,1},{1,2},{-1,2}, {-2,1},
                                        {-2,-1},  {-1,-2}, {1,-2},{2,-1}};
    auto result = std::make_unique<std::vector<FChessPieceStep>>();
    for(FCellIndex target : possibleSteps)
    {
        target =  target + CurrentCell;
        PushStepIfValid(ChessBoardInfo, target, result.get());
    }
    return result;
}

FString UKnight::GetLogView()
{
    if(Color == PIECE_COLOR::WHITE)
    {
        return "N";
    }
    return "n";
}
