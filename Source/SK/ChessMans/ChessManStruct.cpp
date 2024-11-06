// 

// Base:
#include "ChessManStruct.h"

// Interaction:
#include "SK/Core/SK_Character.h"
#include "SK/ChessMans/ChessMan.h"
#include "SK/ChessBoard/Square.h"
//--------------------------------------------------------------------------------------



/* ---   Functions   --- */

AChessMan* GetAvailableChessMan(
    AChessMan* iChessMan,
    const TArray<TArray<ASquare*>>* iAllSquares)
{
    // Поочерёдная проверка валидности указателей
    if (iChessMan
        && iAllSquares
        && iChessMan->CurrentData.Type < EChessManType::NONE)
    {
        // Напрямую получить и подготовить данные Шахматной фигуры через ссылку (&)
        FChessManData& lData = iChessMan->CurrentData;
        lData.AvailablePositions.Empty();

        // 
        TArray<FIndex2D> lSquaresIndex2D;



        switch (lData.Type)
        {
        case EChessManType::Pawn: // Пешка
            lSquaresIndex2D = GetSquareForPawn(
                iChessMan->bFirstMove,
                lData.Position,
                iAllSquares);
            break;

        case EChessManType::Knight: // Конь
            lSquaresIndex2D = GetSquareForKnight(
                lData.Position,
                iAllSquares);
            break;

        case EChessManType::Bishop: // Слон
            lSquaresIndex2D = GetSquareForBishop(
                lData.Position,
                iAllSquares);
            break;

        case EChessManType::Rook: // Ладья
            lSquaresIndex2D = GetSquareForRook(
                lData.Position,
                iAllSquares);
            break;

        case EChessManType::Queen: // Ферзь (Королева)
            lSquaresIndex2D = GetSquareForQueen(
                lData.Position,
                iAllSquares);
            break;
        }


        lData.AvailablePositions += lSquaresIndex2D;
    }

    return nullptr;
};
//--------------------------------------------------------------------------------------



/* ---   Functions | Get Square For ChessMan   --- */
// PS: Local Functions

TArray<FIndex2D> GetSquareForPawn(
    const bool bFirstMove,
    const FIndex2D& iPosition,
    const TArray<TArray<ASquare*>>* iAllSquares)
{
    // Инициализация локальных переменных
    TArray<FIndex2D> lNotChecked;
    TArray<FIndex2D> lResult;
    FSquareData lSquareData; // Данные Клетки

    // Ход вперёд
    lNotChecked.Add(iPosition + FIndex2D { 1, 0 });

    // Длинный ход вперёд
    if (bFirstMove)
    {
        lNotChecked.Add(iPosition + FIndex2D { 2, 0 });
    }

    // Проверка короткого и длинного хода вперёд
    CheckPosition(
        iPosition,
        lNotChecked,
        iAllSquares,
        lResult);

    // Сброс переменной
    lNotChecked.Empty(2);

    // Ход по диагонали
    lNotChecked.Add(iPosition + FIndex2D { 1, 1 });
    lNotChecked.Add(iPosition + FIndex2D { 1, -1 });

    // Проверка хода по диагонали (Индивидуально для Пешки)
    for (auto& lData : lNotChecked)
    {
        if (CheckSquare(iPosition, iAllSquares, lSquareData))
        {
            switch (lSquareData.WarringPartiesType)
            {
            case EWarringPartiesType::White:
                // Требуется добавить Важные ходы (Мат Игроку)
                lResult.Add(lData);
                break;
            }
        }
    }

    return lResult;
}

TArray<FIndex2D> GetSquareForKnight(
    const FIndex2D& iPosition,
    const TArray<TArray<ASquare*>>* iAllSquares)
{
    // Инициализация локальных переменных
    TArray<FIndex2D> lNotChecked;
    lNotChecked.SetNum(8);
    TArray<FIndex2D> lResult;

    // Просчёт ходов Коня
    lNotChecked.Add(iPosition + FIndex2D { 1, 2 });
    lNotChecked.Add(iPosition + FIndex2D { 1, -2 });
    lNotChecked.Add(iPosition + FIndex2D { -1, 2 });
    lNotChecked.Add(iPosition + FIndex2D { -1, -2 });
    //
    lNotChecked.Add(iPosition + FIndex2D { 2, 1 });
    lNotChecked.Add(iPosition + FIndex2D { 2, -1 });
    lNotChecked.Add(iPosition + FIndex2D { -2, 1 });
    lNotChecked.Add(iPosition + FIndex2D { -2, -1 });

    // Проверка ходов Коня
    CheckPosition(
        iPosition,
        lNotChecked,
        iAllSquares,
        lResult);

    return lResult;
}

TArray<FIndex2D> GetSquareForBishop(
    const FIndex2D& iPosition,
    const TArray<TArray<ASquare*>>* iAllSquares)
{
    // Инициализация локальных переменных
    TArray<FIndex2D> lResult;
    FSquareData lSquareData; // Данные Клетки

    // Проверка ходов Слона (+i, +i)
    for (FIndex2D i = iPosition + 1;
        CheckSquare(i, iAllSquares, lSquareData)
        && lSquareData.WarringPartiesType <= EWarringPartiesType::White;
        ++i)
    {
        lResult.Add(i);
    }

    // Проверка ходов Слона (-i, -i)
    for (FIndex2D i = iPosition - 1;
        CheckSquare(i, iAllSquares, lSquareData)
        && lSquareData.WarringPartiesType <= EWarringPartiesType::White;
        --i)
    {
        lResult.Add(i);
    }

    // Проверка ходов Слона (+i, -i)
    for (FIndex2D i = iPosition + FIndex2D { 1, -1 };
        CheckSquare(i, iAllSquares, lSquareData)
        && lSquareData.WarringPartiesType <= EWarringPartiesType::White;
        i += FIndex2D { 1, -1 })
    {
        lResult.Add(i);
    }

    // Проверка ходов Слона (-i, +i)
    for (FIndex2D i = iPosition + FIndex2D { -1, 1 };
        CheckSquare(i, iAllSquares, lSquareData)
        && lSquareData.WarringPartiesType <= EWarringPartiesType::White;
        i += FIndex2D { -1, 1 })
    {
        lResult.Add(i);
    }

    return lResult;
}

TArray<FIndex2D> GetSquareForRook(
    const FIndex2D& iPosition,
    const TArray<TArray<ASquare*>>* iAllSquares)
{
    // Инициализация локальных переменных
    TArray<FIndex2D> lResult;
    FSquareData lSquareData; // Данные Клетки

    // Проверка ходов Слона (+i, 0)
    for (FIndex2D i = iPosition + FIndex2D { 1, 0 };
        CheckSquare(i, iAllSquares, lSquareData)
        && lSquareData.WarringPartiesType <= EWarringPartiesType::White;
        i += FIndex2D { 1, 0 })
    {
        lResult.Add(i);
    }

    // Проверка ходов Слона (-i, 0)
    for (FIndex2D i = iPosition - FIndex2D { -1, 0 };
        CheckSquare(i, iAllSquares, lSquareData)
        && lSquareData.WarringPartiesType <= EWarringPartiesType::White;
        i += FIndex2D { -1, 0 })
    {
        lResult.Add(i);
    }

    // Проверка ходов Слона (0, +i)
    for (FIndex2D i = iPosition + FIndex2D { 0, +1 };
        CheckSquare(i, iAllSquares, lSquareData)
        && lSquareData.WarringPartiesType <= EWarringPartiesType::White;
        i += FIndex2D { 0, +1 })
    {
        lResult.Add(i);
    }

    // Проверка ходов Слона (0, -i)
    for (FIndex2D i = iPosition + FIndex2D { 0, -1 };
        CheckSquare(i, iAllSquares, lSquareData)
        && lSquareData.WarringPartiesType <= EWarringPartiesType::White;
        i += FIndex2D { 0, -1 })
    {
        lResult.Add(i);
    }

    return lResult;
}

TArray<FIndex2D> GetSquareForQueen(
    const FIndex2D& iPosition,
    const TArray<TArray<ASquare*>>* iAllSquares)
{
    // Инициализация локальных переменных
    TArray<FIndex2D> lResult;

    lResult = GetSquareForBishop(
        iPosition,
        iAllSquares);

    lResult += GetSquareForRook(
        iPosition,
        iAllSquares);

    return lResult;
}
//--------------------------------------------------------------------------------------



/* ---   Functions | Tools   --- */

void CheckPosition(
    const FIndex2D& iPosition,
    const TArray<FIndex2D>& iNotChecked,
    const TArray<TArray<ASquare*>>* iAllSquares,
    TArray<FIndex2D> oResult)
{
    FSquareData lSquareData; // Данные Клетки

    for (auto& lData : iNotChecked)
    {
        if (CheckSquare(iPosition, iAllSquares, lSquareData))
        {
            switch (lSquareData.WarringPartiesType)
            {
            case EWarringPartiesType::White:
                // Требуется добавить Важные ходы (Мат Игроку)
            case EWarringPartiesType::NONE:
                oResult.Add(lData);
                break;
            }
        }
    }
}

bool CheckSquare(
    const FIndex2D& iPosition,
    const TArray<TArray<ASquare*>>* iAllSquares,
    FSquareData& oSquareData)
{
    if (iAllSquares->IsValidIndex(iPosition.X)
        && (*iAllSquares)[iPosition.X].IsValidIndex(iPosition.Y))
    {
        ASquare* lSquare = ((*iAllSquares)[iPosition.X])[iPosition.Y];

        if (lSquare)
        {
            oSquareData = lSquare->GetData();
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------
