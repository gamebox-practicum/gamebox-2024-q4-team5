// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSquareInfo.h"
#include "SK/Tools/Index2D.h"
#include "UObject/Object.h"
#include "ChessBoardInfo.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class SK_API UChessBoardInfo : public UObject
{
    GENERATED_BODY()
private:
    FSquareInfo** m_Board = nullptr;
    int m_sizeX;
    int m_sizeY;
public:
    UFUNCTION(BlueprintCallable)
    void Init(int sizeY, int sizeX);

    UFUNCTION(BlueprintCallable)
    FSquareInfo Get(int Y, int X);

    UFUNCTION(BlueprintCallable)
    void Set(int Y, int X, FSquareInfo value);

    FSquareInfo*& operator[] (int Y);

    virtual ~UChessBoardInfo() override;
};
