// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SK/Tools/Chess_AI/UChessPieceInfo.h"
#include "King.generated.h"

/**
 *
 */
UCLASS()
class SK_API UKing : public UChessPieceInfo
{
    GENERATED_BODY()
public:
    virtual float GetRelativeValue() override;

    virtual std::unique_ptr<std::vector<FChessPieceStep>> GetLegalMoves(UChessBoardInfo* ChessBoardInfo) override;

    virtual FString GetLogView() override;
};
