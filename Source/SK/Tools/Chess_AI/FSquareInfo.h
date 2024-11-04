#pragma once
#include "UChessPieceInfo.h"
#include  "FSquareInfo.generated.h"

USTRUCT(BlueprintType)
struct FSquareInfo
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    UChessPieceInfo* CurrentPiece;

    UPROPERTY(BlueprintReadWrite)
    bool IsEmpty = true;
};
