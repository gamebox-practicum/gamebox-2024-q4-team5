// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SK/Tools/Chess_AI/UChessPieceInfo.h"
#include "Bishop.generated.h"

/**
 *
 */
UCLASS()
class SK_API UBishop : public UChessPieceInfo
{
    GENERATED_BODY()
public:
    virtual float GetRelativeValue() override;

    virtual FString GetLogView() override;

    virtual std::unique_ptr<std::vector<FChessPieceStep>> GetLegalMoves(UChessBoardInfo* ChessBoardInfo) override;
};
