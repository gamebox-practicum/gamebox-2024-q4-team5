// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessBoardInfo.h"
#include "UChessPieceInfo.h"
#include "FChessPieceStep.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ChessAILibrary.generated.h"


UDELEGATE()
    DECLARE_DYNAMIC_DELEGATE_OneParam(FOnStepCalculatedSignature, FChessPieceStep, Response);
/**
 *
 */
UCLASS()
class SK_API UChessAILibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "ChessAI")
    static FChessPieceStep GetNextStep(UChessBoardInfo* ChessBoardInfo, PIECE_COLOR CurrentStepColor, int depth);

    UFUNCTION(BlueprintCallable, Category = "ChessAI")
    static void LogChessBoard(UChessBoardInfo* ChessBoardInfo);

    UFUNCTION(BlueprintCallable, Category = "ChessAI")
    static void DoStep(FChessPieceStep Step, UChessBoardInfo* ChessBoardInfo);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ChessAI")
    static bool IsUnableMove(FChessPieceStep Step){return Step == UNABLE_MOVE;}

    UFUNCTION(BlueprintCallable, Category = "ChessAI", meta = (AutoCreateRefTerm = "OnProcessRequestComplete"))
    static void GetNextStepAsync(UChessBoardInfo* ChessBoardInfo, PIECE_COLOR CurrentStepColor, int depth,
        const FOnStepCalculatedSignature& OnProcessRequestComplete);

    ///Utils---------------

private:

    static FChessPieceStep GetBestStep(UChessBoardInfo* ChessBoardInfo,
        std::vector<UChessPieceInfo*>& AttackingFigures,
        std::vector<UChessPieceInfo*>& DefensiveFigures,
        int depth,
        float& TotalScore);

    static constexpr float DepthCoefficient = 0.999f;

    static constexpr int LastLineBonus = 100;

    static void DoStep(FChessPieceStep Step, UChessBoardInfo* ChessBoardInfo,
    std::vector<UChessPieceInfo*>& DefensiveFigures);

    static void UndoStep(FChessPieceStep Step, UChessBoardInfo* ChessBoardInfo,
    std::vector<UChessPieceInfo*>& DefensiveFigures);

    template<typename T>
    static void CopyTArrayToVector(TArray<T>& Array, std::vector<T>& vector);

    //static FChessPieceStep GetBestStep(UChessPieceInfo* Piece, UChessBoardInfo* ChessBoardInfo, int& score);
};

