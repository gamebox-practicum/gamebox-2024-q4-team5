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

    UFUNCTION(BlueprintCallable, Category = "ChessAI")
    static FChessPieceStep GetNextStep(UChessBoardInfo* ChessBoardInfo, PIECE_COLOR CurrentStepColor);
};
