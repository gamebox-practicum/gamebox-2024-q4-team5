#pragma once
#include "Chess_AI/UChessPieceInfo.h"
#include "Chess_AI/ChessPieces/Bishop.h"
#include "Chess_AI/ChessPieces/King.h"
#include "Chess_AI/ChessPieces/Knight.h"
#include "Chess_AI/ChessPieces/PawnChess.h"
#include "Chess_AI/ChessPieces/Queen.h"
#include "Chess_AI/ChessPieces/Rook.h"
#include "SK/ChessMans/ChessManStruct.h"

enum struct EChessManType : uint8;

class SKUtils
{
public:
    static UChessPieceInfo* ConstructChessPiece(EChessManType Type, PIECE_COLOR Color, UObject* Owner)
    {
        UChessPieceInfo* result;

        switch (Type)
        {
        case EChessManType::Bishop:
            result = NewObject<UBishop>(Owner);
            break;
        case EChessManType::Knight:
            result = NewObject<UKnight>(Owner);
            break;
        case EChessManType::Pawn:
            result = NewObject<UPawnChess>(Owner);
            break;
        case EChessManType::Queen:
            result = NewObject<UQueen>(Owner);
            break;
        case EChessManType::Rook:
            result = NewObject<URook>(Owner);
            break;
        case EChessManType::King:
            result = NewObject<UKing>(Owner);
            break;
        default:
            return nullptr;
        }

        result->Color = Color;

        return result;
    }
};
