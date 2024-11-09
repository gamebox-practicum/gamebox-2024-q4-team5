// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessBoardInfo.h"

void UChessBoardInfo::Clear()
{
    if(m_Board == nullptr)
    {
        return;
    }
    for (int count = 0; count < m_sizeY; ++count)
    {
        delete[] m_Board[count];
    }
    delete[] m_Board;

    whitePieces.Empty();
    blackPieces.Empty();
}

void UChessBoardInfo::Init( int sizeY, int sizeX)
{
    Clear();

    m_sizeY = sizeY;
    m_sizeX = sizeX;
    m_Board = new FSquareInfo*[sizeY];
    for (int count = 0; count < sizeY; ++count)
    {
        m_Board[count] = new FSquareInfo[sizeX];
    }
}

void UChessBoardInfo::Set(int Y, int X, UChessPieceInfo* value)
{
    m_Board[Y][X].CurrentPiece = value;
    value->CurrentCell.Y = Y;
    value->CurrentCell.X = X;

    if(value->Color==PIECE_COLOR::WHITE)
    {
        whitePieces.AddUnique(value);
    }
    if(value->Color==PIECE_COLOR::BLACK)
    {
        blackPieces.AddUnique(value);
    }
}

FSquareInfo*& UChessBoardInfo::operator[](int Y)
{
    return m_Board[Y];
}

FSquareInfo& UChessBoardInfo::operator[](FCellIndex cell)
{
    return  (*this)[cell.Y][cell.X];
}

UChessBoardInfo::~UChessBoardInfo()
{
    Clear();
}
