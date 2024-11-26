#pragma once
#include <memory>
#include <string>
#include <vector>

#include "FCellIndex.h"
#include "FChessPieceStep.h"
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
    virtual std::unique_ptr<std::vector<FChessPieceStep>> GetLegalMoves(UChessBoardInfo* ChessBoardInfo);

    virtual float GetRelativeValue();

    virtual ~UChessPieceInfo() override{};

    virtual FString GetLogView();

    //service variable for UChessAILibrary::GetBestStep
    bool IsDead = false;

protected:
    void PushStepIfValid(UChessBoardInfo* ChessBoardInfo, FCellIndex Target,
    std::vector<FChessPieceStep>* Result);

    bool CanJumpOver(UChessBoardInfo* ChessBoardInfo, FCellIndex cell);
};


