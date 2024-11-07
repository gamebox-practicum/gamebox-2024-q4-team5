// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessAILibrary.h"

FChessPieceStep UChessAILibrary::GetNextStep(UChessBoardInfo* ChessBoardInfo, PIECE_COLOR CurrentStepColor)
{
    std::vector<UChessPieceInfo*> whitePieces;
    std::vector<UChessPieceInfo*> blackPieces;

    GetPieces(ChessBoardInfo, whitePieces, blackPieces);



    FChessPieceStep result;
    return result;
}

void UChessAILibrary::GetPieces(UChessBoardInfo* ChessBoardInfo, std::vector<UChessPieceInfo*>& WhitePieces,
    std::vector<UChessPieceInfo*>& BlackPieces)
{
    int sizeY = ChessBoardInfo->GetSizeY();
    int sizeX = ChessBoardInfo->GetSizeX();
    for(int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            auto currentPiece = (*ChessBoardInfo)[y][x].CurrentPiece;
            if(currentPiece)
            {
                if(currentPiece->Color == PIECE_COLOR::WHITE)
                {
                    WhitePieces.push_back(currentPiece);
                }
                if(currentPiece->Color == PIECE_COLOR::BLACK)
                {
                    BlackPieces.push_back(currentPiece);
                }
            }
        }
    }
}

FChessPieceStep UChessAILibrary::GetBestStep(UChessBoardInfo* ChessBoardInfo,
    std::vector<UChessPieceInfo*>& AttackingFigures, std::vector<UChessPieceInfo*>& DefensiveFigures, int depth,
    int& TotalScore)
{
    for (auto figure : AttackingFigures)
    {

    }
    return  UNABLE_MOVE;
}

FChessPieceStep UChessAILibrary::GetBestStep(UChessPieceInfo* Piece, UChessBoardInfo* ChessBoardInfo, int& score)
{
    auto legalMoves = *(Piece->GetLegalMoves(ChessBoardInfo));
    FChessPieceStep Result = UNABLE_MOVE;

    int bestScore = -1;

    for (FCellIndex Move : legalMoves)
    {
        UChessPieceInfo* targetPiece = (*ChessBoardInfo)[Move.Y][Move.X].CurrentPiece;
        int currentScore = 0;

        if(targetPiece)
        {
            if(targetPiece->Color == Piece->Color)
            {
                continue;
            }
            else
            {
                currentScore = targetPiece->GetRelativeValue();
            }
        }

        if(currentScore > bestScore)
        {
            bestScore = currentScore;
            Result.PreviousPosition = Piece->CurrentCell;
            Result.NewPosition = Move;
            Result.AttackedPiece = targetPiece;
        }
    }

    score = bestScore;
    return  Result;
}
