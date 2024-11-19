// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnChess.h"

#include "SK/Tools/Chess_AI/ChessBoardInfo.h"

float UPawnChess::GetRelativeValue()
{
    return 1;
}

void UPawnChess::PushAttackStepIfValid(UChessBoardInfo* ChessBoardInfo, std::vector<FChessPieceStep>* result, FCellIndex attackTarget)
{
    if(ChessBoardInfo->IsValidCell(attackTarget) &&
        (*ChessBoardInfo)[attackTarget].IsCanStepOn)
    {
        auto targetPiece = (*ChessBoardInfo)[attackTarget].CurrentPiece;
        if(targetPiece && targetPiece->Color != this->Color)
        {
            result->push_back({this->CurrentCell, attackTarget, targetPiece});
        }
    }
}

void UPawnChess::PushNonAttackStepIfValid(UChessBoardInfo* ChessBoardInfo, std::vector<FChessPieceStep>* result,
    FCellIndex target)
{
    if(ChessBoardInfo->IsValidCell(target) &&
        (*ChessBoardInfo)[target].CurrentPiece == nullptr &&
        (*ChessBoardInfo)[target].IsCanStepOn)
    {
        result->push_back({this->CurrentCell, target, nullptr});
    }
}

std::unique_ptr<std::vector<FChessPieceStep>> UPawnChess::GetLegalMoves(UChessBoardInfo* ChessBoardInfo)
{
    auto result = std::make_unique<std::vector<FChessPieceStep>>();

    ///!!!white pawns move towards increasing Y, black pawns move towards decreasing Y
    if(Color == PIECE_COLOR::WHITE)
    {
        FCellIndex target = CurrentCell + FCellIndex{1, -1};
        PushAttackStepIfValid(ChessBoardInfo, result.get(), target);
        target = CurrentCell + FCellIndex{1, 1};
        PushAttackStepIfValid(ChessBoardInfo, result.get(), target);

        target = CurrentCell + FCellIndex{1, 0};
        PushNonAttackStepIfValid(ChessBoardInfo, result.get(), target);
    }
    if(Color == PIECE_COLOR::BLACK)
    {
        FCellIndex target = CurrentCell + FCellIndex{-1, -1};
        PushAttackStepIfValid(ChessBoardInfo, result.get(), target);
        target = CurrentCell + FCellIndex{-1, 1};
        PushAttackStepIfValid(ChessBoardInfo, result.get(), target);

        target = CurrentCell + FCellIndex{-1, 0};
        PushNonAttackStepIfValid(ChessBoardInfo, result.get(), target);
    }

    return result;
}

FString UPawnChess::GetLogView()
{
    if(Color == PIECE_COLOR::WHITE)
    {
        return "P";
    }
    return "p";
}
