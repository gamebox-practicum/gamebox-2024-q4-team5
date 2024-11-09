// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnChess.h"

#include "SK/Tools/Chess_AI/ChessBoardInfo.h"

int UPawnChess::GetRelativeValue()
{
    return 1;
}

void UPawnChess::PushAttackStepIfValid(UChessBoardInfo* ChessBoardInfo, std::vector<FChessPieceStep>* result, FCellIndex attackTarget)
{
    if(ChessBoardInfo->IsValidCell(attackTarget))
    {
        auto targetPiece = (*ChessBoardInfo)[attackTarget].CurrentPiece;
        if(targetPiece && targetPiece->Color != this->Color)
        {
            result->push_back({this->CurrentCell, attackTarget, targetPiece});
        }
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
        //doesn't account for the corpses yet.
        if(ChessBoardInfo->IsValidCell(target) && (*ChessBoardInfo)[target].CurrentPiece == nullptr)
        {
            result.get()->push_back({this->CurrentCell, target, nullptr});
        }
    }
    if(Color == PIECE_COLOR::BLACK)
    {
        FCellIndex target = CurrentCell + FCellIndex{-1, -1};
        PushAttackStepIfValid(ChessBoardInfo, result.get(), target);
        target = CurrentCell + FCellIndex{-1, 1};
        PushAttackStepIfValid(ChessBoardInfo, result.get(), target);

        target = CurrentCell + FCellIndex{-1, 0};
        //doesn't account for the corpses yet.
        if(ChessBoardInfo->IsValidCell(target) && (*ChessBoardInfo)[target].CurrentPiece == nullptr)
        {
            result.get()->push_back({this->CurrentCell, target, nullptr});
        }
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
