// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessAILibrary.h"

#include <iterator>
#include <stdexcept>

FChessPieceStep UChessAILibrary::GetNextStep(UChessBoardInfo* ChessBoardInfo, PIECE_COLOR CurrentStepColor, int depth)
{
    std::vector<UChessPieceInfo*> whitePieces;
    std::vector<UChessPieceInfo*> blackPieces;
    CopyTArrayToVector(ChessBoardInfo->whitePieces, whitePieces);
    CopyTArrayToVector(ChessBoardInfo->blackPieces, blackPieces);

    ///if there are no figures in any team, return UNABLE_MOVE
        if((whitePieces.size()) <=0 || (whitePieces.size() <= 0))
        {
            return UNABLE_MOVE;
        }
    ///

    float score;
    if(CurrentStepColor == PIECE_COLOR::WHITE)
    {
        return GetBestStep(ChessBoardInfo, whitePieces, blackPieces, depth, score);
    }
    else
    {
        auto temp = GetBestStep(ChessBoardInfo, blackPieces, whitePieces, depth, score);
        UE_LOG(LogTemp, Warning,
                TEXT("%f"), score);
        return temp;
    }
}

void UChessAILibrary::LogChessBoard(UChessBoardInfo* ChessBoardInfo)
{
    for(int y = 0; y < ChessBoardInfo->GetSizeY(); y++)
    {
        FString str = "";
        for( int x = 0; x < ChessBoardInfo-> GetSizeX(); x++)
        {
            auto piece = (*ChessBoardInfo)[y][x].CurrentPiece;
            if(piece)
            {
                str+= piece->GetLogView();
            }
            else
            {
                str+= " ";
            }
        }
        UE_LOG(LogTemp, Warning, TEXT("%s"), *str);
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

    if(Step.AttackedPiece)
    {
        if(Step.AttackedPiece->Color == PIECE_COLOR::WHITE)
        {
            ChessBoardInfo->whitePieces.Remove(Step.AttackedPiece);
        }
        if(Step.AttackedPiece->Color == PIECE_COLOR::BLACK)
        {
            ChessBoardInfo->blackPieces.Remove(Step.AttackedPiece);
        }
    }
}

void UChessAILibrary::GetNextStepAsync(UChessBoardInfo* ChessBoardInfo, PIECE_COLOR CurrentStepColor, int depth,
    const FOnStepCalculatedSignature& OnProcessRequestComplete)
{
    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask,
        [ChessBoardInfo, CurrentStepColor, depth, OnProcessRequestComplete] ()
    {
        FChessPieceStep result = GetNextStep(ChessBoardInfo, CurrentStepColor, depth);

        AsyncTask(ENamedThreads::GameThread,
            [OnProcessRequestComplete, result] ()
        {
            OnProcessRequestComplete.ExecuteIfBound(result);
        });
    });
}

FChessPieceStep UChessAILibrary::GetBestStep(UChessBoardInfo* ChessBoardInfo,
    std::vector<UChessPieceInfo*>& AttackingFigures, std::vector<UChessPieceInfo*>& DefensiveFigures, int depth,
    float& TotalScore)
{
    float BestScore = - std::numeric_limits<float>::max();
    FChessPieceStep Result = UNABLE_MOVE;
    depth--;

    for (auto figure : AttackingFigures)
    {
        //ugly on application ending check
        if(!IsValid(figure) || !IsValid(ChessBoardInfo))
        {
            UE_LOG(LogTemp, Warning,
                TEXT("UChessAILibrary::GetBestStep: !IsValid(figure) || !IsValid(ChessBoardInfo)"));
            return Result;
        }

        auto steps{*((figure->GetLegalMoves(ChessBoardInfo)).get())};
        for (auto step : steps)
        {
            float currentScore = step.GetStepScore();

            if(depth > 0)
            {
                DoStep(step, ChessBoardInfo, DefensiveFigures);

                float defensiveBestScore;
                const FChessPieceStep defensiveBestStep = GetBestStep(ChessBoardInfo, DefensiveFigures,
                AttackingFigures, depth, defensiveBestScore);
                if(defensiveBestStep != UNABLE_MOVE)
                {
                    currentScore -= defensiveBestScore * DepthCoefficient;
                }

                UndoStep(step, ChessBoardInfo, DefensiveFigures);
            }

            if((currentScore > BestScore) || ((currentScore == BestScore) && FMath::RandBool()) )
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
        //throw std::runtime_error("UChessAILibrary::DoStep: invalid Step value");
        UE_LOG(LogTemp, Error,
                TEXT("UChessAILibrary::DoStep: invalid Step value"));
    }
    (*ChessBoardInfo)[Step.PreviousPosition].CurrentPiece = nullptr;
    ChessBoardInfo->Set(Step.NewPosition.Y, Step.NewPosition.X, piece);

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

    if((!piece) || emptyCellPiece)
    {
        //throw std::runtime_error("UChessAILibrary::UndoStep: invalid Step value");
        UE_LOG(LogTemp, Error,
            TEXT("UChessAILibrary::UndoStep: invalid Step value"));
    }
    ChessBoardInfo->Set(Step.PreviousPosition.Y, Step.PreviousPosition.X, piece);
    (*ChessBoardInfo)[Step.NewPosition].CurrentPiece = nullptr;

    if(Step.AttackedPiece)
    {
        ChessBoardInfo->Set(Step.NewPosition.Y, Step.NewPosition.X, Step.AttackedPiece);
        DefensiveFigures.push_back(Step.AttackedPiece);
    }
}

template <typename T>
void UChessAILibrary::CopyTArrayToVector(TArray<T>& Array, std::vector<T>& vector)
{
    for (auto Element : Array)
    {
        vector.push_back(Element);
    }
}
