#pragma once
#include <memory>
#include <vector>

#include "FCellIndex.h"
#include "UChessPieceInfo.generated.h"

class UChessBoardInfo;

UENUM(BlueprintType)
enum class PIECE_COLOR : uint8{
    WHITE = 0 UMETA(DisplayName = "WHITE"),
    BLACK = 1 UMETA(DisplayName = "BLACK")
};

///
///базовый класс хранения поведения для шахматных фигур
///
UCLASS(BlueprintType)
class SK_API  UChessPieceInfo: public UObject
{
    GENERATED_BODY()

public:
    FCellIndex CurrentCell;

    UPROPERTY(BlueprintReadWrite)
    PIECE_COLOR Color;

    /// возвращает точки в которые фигура может сходить в данный момент
    /// @param ChessBoardInfo
    /// @return
    virtual std::unique_ptr<std::vector<FCellIndex>> GetLegalMoves(UChessBoardInfo* ChessBoardInfo);

    virtual int GetRelativeValue();

    virtual ~UChessPieceInfo() override{};
};


