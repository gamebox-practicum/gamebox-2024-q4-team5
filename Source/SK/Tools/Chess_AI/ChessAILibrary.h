// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessBoardInfo.h"
#include "UChessPieceInfo.h"
#include "FChessPieceStep.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ChessAILibrary.generated.h"

/**
 *
 */
UCLASS()
class SK_API UChessAILibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    ///TODO: предотвратить удаление UChessBoardInfo и UChessPieceInfo during garbage collection
    UFUNCTION(BlueprintCallable, Category = "ChessAI")
    static FChessPieceStep GetNextStep(UChessBoardInfo* ChessBoardInfo, PIECE_COLOR CurrentStepColor);



    ///Utils---------------


    static void GetPieces(UChessBoardInfo* ChessBoardInfo, std::vector<UChessPieceInfo*>& WhitePieces,
        std::vector<UChessPieceInfo*>& BlackPieces);

    static FChessPieceStep GetBestStep(UChessBoardInfo* ChessBoardInfo,
        std::vector<UChessPieceInfo*>& AttackingFigures,
        std::vector<UChessPieceInfo*>& DefensiveFigures,
        int depth,
        int& TotalScore);

    static FChessPieceStep GetBestStep(UChessPieceInfo* Piece, UChessBoardInfo* ChessBoardInfo, int& score);
};
