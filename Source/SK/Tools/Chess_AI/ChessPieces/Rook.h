// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SK/Tools/Chess_AI/UChessPieceInfo.h"
#include "Rook.generated.h"

/**
 *
 */
UCLASS()
class SK_API URook : public UChessPieceInfo
{
    GENERATED_BODY()
public:
    virtual int GetRelativeValue() override;

    virtual FString GetLogView() override;

    virtual std::unique_ptr<std::vector<FChessPieceStep>> GetLegalMoves(UChessBoardInfo* ChessBoardInfo) override;
};
