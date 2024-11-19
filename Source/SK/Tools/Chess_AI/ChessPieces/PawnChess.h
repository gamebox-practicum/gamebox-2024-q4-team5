// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SK/Tools/Chess_AI/UChessPieceInfo.h"
#include "PawnChess.generated.h"

/**
 *
 */
UCLASS()
class SK_API UPawnChess : public UChessPieceInfo
{
    GENERATED_BODY()
public:
    virtual float GetRelativeValue() override;

    virtual std::unique_ptr<std::vector<FChessPieceStep>> GetLegalMoves(UChessBoardInfo* ChessBoardInfo) override;

    virtual FString GetLogView() override;

private:
    void PushAttackStepIfValid(UChessBoardInfo* ChessBoardInfo, std::vector<FChessPieceStep>* result,
                               FCellIndex attackTarget);

    void PushNonAttackStepIfValid(UChessBoardInfo* ChessBoardInfo, std::vector<FChessPieceStep>* result,
                               FCellIndex target);
};
