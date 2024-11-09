// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSquareInfo.h"
#include "UChessPieceInfo.h"
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
    void Set(int Y, int X, UChessPieceInfo* value);

    UFUNCTION(BlueprintCallable)
    int GetSizeY(){return m_sizeY;}

    UFUNCTION(BlueprintCallable)
    int GetSizeX(){return m_sizeX;}

    FSquareInfo*& operator[] (int Y);

    FSquareInfo& operator[] (FCellIndex cell);

    virtual ~UChessBoardInfo() override;

    bool IsValidCell(FCellIndex cell);
};

inline bool UChessBoardInfo::IsValidCell(FCellIndex cell)
{
    if(cell.Y < m_sizeY && cell.Y >= 0 && cell.X < m_sizeX && cell.X >= 0)
    {
        return true;
    }
    return false;
}
