// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessAILibrary.h"

#include <stdexcept>

FChessPieceStep UChessAILibrary::GetNextStep(UChessBoardInfo* ChessBoardInfo, PIECE_COLOR CurrentStepColor, int depth)
{
    std::vector<UChessPieceInfo*> whitePieces;
    std::vector<UChessPieceInfo*> blackPieces;

    GetPieces(ChessBoardInfo, whitePieces, blackPieces);

    ///testing: if there are no figures in any team, return UNABLE_MOVE
        if((whitePieces.size()) <=0 || (whitePieces.size() <= 0))
        {
            return UNABLE_MOVE;
        }
    ///

    int score;
    if(CurrentStepColor == PIECE_COLOR::WHITE)
    {
        return GetBestStep(ChessBoardInfo, whitePieces, blackPieces, depth, score);
    }
    else
    {
        return GetBestStep(ChessBoardInfo, blackPieces, whitePieces, depth, score);
    }
}

void UChessAILibrary::LogChessBoard(UChessBoardInfo* ChessBoardInfo)
{
    for(int y = 0; y < ChessBoardInfo->GetSizeY(); y++)
    {
        FString str = "";
        for( int x = 0; x < ChessBoardInfo-> GetSizeX(); x++)
        {
            if((*ChessBoardInfo)[y][x].CurrentPiece)
            {
                str+= "8";
            }
            else
            {
                str+= "O";
            }
        }
        UE_LOG(LogTemp, Warning, TEXT("The vector value is: %s"), *str);
    }

    UE_LOG(LogTemp, Warning, TEXT("---------------------"));
}

void UChessAILibrary::DoStep(FChessPieceStep Step, UChessBoardInfo* ChessBoardInfo)
{
    auto piece = (*ChessBoardInfo)[Step.PreviousPosition].CurrentPiece;
    if(!piece)
    {
        throw std::runtime_error("UChessAILibrary::DoStep: invalid Step value");
    }
    (*ChessBoardInfo)[Step.PreviousPosition].CurrentPiece = nullptr;
    ChessBoardInfo->Set(Step.NewPosition.Y, Step.NewPosition.X, piece);
    //(*ChessBoardInfo)[Step.NewPosition].CurrentPiece = piece;
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
    int BestScore = -1;
    FChessPieceStep Result = UNABLE_MOVE;
    depth--;

    for (auto figure : AttackingFigures)
    {
        auto tmp = (figure->GetLegalMoves(ChessBoardInfo));
        auto tmp2 = tmp.get();
        auto steps = *(tmp2);
        for (auto step : steps)
        {
            int currentScore = step.GetStepScore();

            if(depth > 0)
            {
                DoStep(step, ChessBoardInfo, DefensiveFigures);

                int defensiveBestScore;
                const FChessPieceStep defensiveBestStep = GetBestStep(ChessBoardInfo, DefensiveFigures,
                AttackingFigures, depth, defensiveBestScore);
                if(defensiveBestStep != UNABLE_MOVE)
                {
                    currentScore -= defensiveBestScore;
                }

                UndoStep(step, ChessBoardInfo, DefensiveFigures);
            }

            if(currentScore > BestScore)
            {
                BestScore = currentScore;
                Result = step;
            }
        }
    }

    TotalScore = BestScore;
    return  Result;
}

void UChessAILibrary::DoStep(FChessPieceStep Step, UChessBoardInfo* ChessBoardInfo,
    std::vector<UChessPieceInfo*>& DefensiveFigures)
{
    auto piece = (*ChessBoardInfo)[Step.PreviousPosition].CurrentPiece;
    if(!piece)
    {
        throw std::runtime_error("UChessAILibrary::DoStep: invalid Step value");
    }
    (*ChessBoardInfo)[Step.PreviousPosition].CurrentPiece = nullptr;
    ChessBoardInfo->Set(Step.NewPosition.Y, Step.NewPosition.X, piece);
    //(*ChessBoardInfo)[Step.NewPosition].CurrentPiece = piece;

    if(Step.AttackedPiece)
    {
        std::_Erase_remove(DefensiveFigures, Step.AttackedPiece);
    }
}

void UChessAILibrary::UndoStep(FChessPieceStep Step, UChessBoardInfo* ChessBoardInfo,
    std::vector<UChessPieceInfo*>& DefensiveFigures)
{
    auto piece = (*ChessBoardInfo)[Step.NewPosition].CurrentPiece;
    auto emptyCellPiece = (*ChessBoardInfo)[Step.PreviousPosition].CurrentPiece;

    //UE_LOG(LogTemp, Error, TEXT("%s"), piece == nullptr? TEXT("piece = nullptr"): TEXT("piece is valid"));
    //UE_LOG(LogTemp, Error, TEXT("%s"), emptyCellPiece == nullptr? TEXT("emptyCellPiece = nullptr"): TEXT("emptyCellPiece is valid"));
    if((!piece) || emptyCellPiece)
    {
        throw std::runtime_error("UChessAILibrary::UndoStep: invalid Step value");
    }
    ChessBoardInfo->Set(Step.PreviousPosition.Y, Step.PreviousPosition.X, piece);
    (*ChessBoardInfo)[Step.NewPosition].CurrentPiece = nullptr;
    //(*ChessBoardInfo)[Step.PreviousPosition].CurrentPiece = piece;

    if(Step.AttackedPiece)
    {
        ChessBoardInfo->Set(Step.NewPosition.Y, Step.NewPosition.X, Step.AttackedPiece);
        DefensiveFigures.push_back(Step.AttackedPiece);
    }
}

/*
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
}*/
