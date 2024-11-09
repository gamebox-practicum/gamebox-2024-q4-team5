// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SK/Tools/Chess_AI/UChessPieceInfo.h"
#include "Knight.generated.h"

/**
 *
 */
UCLASS()
class SK_API UKnight : public UChessPieceInfo
{
    GENERATED_BODY()
public:
    virtual int GetRelativeValue() override;

    virtual std::unique_ptr<std::vector<FChessPieceStep>> GetLegalMoves(UChessBoardInfo* ChessBoardInfo) override;

    virtual FString GetLogView() override;
};
