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
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans)
{
    // Поочерёдная проверка валидности указателей
    if (iChessMan
        && iAllSquares
        && iChessMan->CurrentData.Type < EChessManType::NONE)
    {
        int32 lCheckAttacking = oAttackingChessMans.Num();

        // Напрямую получить и подготовить данные Шахматной фигуры через ссылку (&)
        FChessManData& lData = iChessMan->CurrentData;
        lData.AvailablePositions.Empty();

        // Массив доступных ходов
        TArray<FIntPoint> lSquaresIndex2D;

        switch (lData.Type)
        {
        case EChessManType::Pawn: // Пешка
            lSquaresIndex2D = GetSquareForPawn(
                lData.Position,
                iAllSquares,
                oAttackingChessMans);
            break;

        case EChessManType::Knight: // Конь
            lSquaresIndex2D = GetSquareForKnight(
                lData.Position,
                iAllSquares,
                oAttackingChessMans);
            break;

        case EChessManType::Bishop: // Слон
            lSquaresIndex2D = GetSquareForBishop(
                lData.Position,
                iAllSquares,
                oAttackingChessMans);
            break;

        case EChessManType::Rook: // Ладья
            lSquaresIndex2D = GetSquareForRook(
                lData.Position,
                iAllSquares,
                oAttackingChessMans);
            break;

        case EChessManType::Queen: // Ферзь (Королева)
            lSquaresIndex2D = GetSquareForQueen(
                lData.Position,
                iAllSquares,
                oAttackingChessMans);
            break;
        }


        if (lSquaresIndex2D.IsValidIndex(0))
        {
            lData.AvailablePositions += lSquaresIndex2D;

            // Если были добавлены позиции атаки, то заполнить указатели на текущую Шахматную фигуру
            if (lCheckAttacking != oAttackingChessMans.Num())
            {
                for (int32 i = lCheckAttacking; i < oAttackingChessMans.Num(); ++i)
                {
                    oAttackingChessMans[i].ChessMan = iChessMan;
                }
            }

            return iChessMan;
        }
    }

    return nullptr;
};
//--------------------------------------------------------------------------------------



/* ---   Functions | Get Square For ChessMan   --- */
// PS: Local Functions

TArray<FIntPoint> GetSquareForPawn(
    const FIntPoint& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans)
{
    // Инициализация локальных переменных
    TArray<FIntPoint> lNotChecked;
    TArray<FIntPoint> lResult;
    FSquareData lSquareData; // Данные Клетки

    // Ход вперёд
    lNotChecked.Add(iPosition + FIntPoint { -1, 0 });

    // Проверка хода по диагонали (Индивидуально для Пешки)
    for (auto& dPosition : lNotChecked)
    {
        if (CheckSquare(dPosition, iAllSquares, lSquareData))
        {
            if (lSquareData.WarringPartiesType == EWarringPartiesType::NONE)
            {
                lResult.Add(dPosition);
            }
        }
    }

    // Сброс переменной
    lNotChecked.Empty(2);

    // Ход по диагонали
    lNotChecked.Add(iPosition + FIntPoint { -1, 1 });
    lNotChecked.Add(iPosition + FIntPoint { -1, -1 });

    // Проверка хода по диагонали (Индивидуально для Пешки)
    for (auto& dPosition : lNotChecked)
    {
        if (CheckSquare(dPosition, iAllSquares, lSquareData))
        {
            if (lSquareData.WarringPartiesType == EWarringPartiesType::White)
            {
                lResult.Add(dPosition);
                oAttackingChessMans.Add(FAttackingChessMansData(nullptr, dPosition));
            }
        }
    }

    return lResult;
}

TArray<FIntPoint> GetSquareForKnight(
    const FIntPoint& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans)
{
    // Инициализация локальных переменных
    TArray<FIntPoint> lNotChecked;
    lNotChecked.Reset(8);
    TArray<FIntPoint> lResult;

    // Просчёт ходов Коня
    lNotChecked.Add(iPosition + FIntPoint { 1, 2 });
    lNotChecked.Add(iPosition + FIntPoint { 1, -2 });
    lNotChecked.Add(iPosition + FIntPoint { -1, 2 });
    lNotChecked.Add(iPosition + FIntPoint { -1, -2 });
    //
    lNotChecked.Add(iPosition + FIntPoint { 2, 1 });
    lNotChecked.Add(iPosition + FIntPoint { 2, -1 });
    lNotChecked.Add(iPosition + FIntPoint { -2, 1 });
    lNotChecked.Add(iPosition + FIntPoint { -2, -1 });

    // Проверка ходов Коня
    CheckPositions(
        lNotChecked,
        iAllSquares,
        lResult,
        oAttackingChessMans);

    return lResult;
}

TArray<FIntPoint> GetSquareForBishop(
    const FIntPoint& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans)
{
    // Инициализация локальных переменных
    TArray<FIntPoint> lResult;
    FSquareData lSquareData; // Данные Клетки
    FIntPoint i;

    // Проверяемые направления для Слона
    TArray<FIntPoint> lCheckedDirections =
    {
        FIntPoint{ 1, 1 },
        FIntPoint { 1, -1 },
        FIntPoint { -1, -1 },
        FIntPoint { -1, 1 }
    };

    // Флаг выхода из цикла проверки конкретного направления
    bool bCheckDirection = false;

    // Выбор проверяемого направления по порядку
    for (auto& lDirection : lCheckedDirections)
    {
        bCheckDirection = false;

        // Проверка ходов Слона в текущем направлении
        for (i = iPosition + lDirection;
            CheckSquare(i, iAllSquares, lSquareData);
            i += lDirection)
        {
            switch (lSquareData.WarringPartiesType)
            {
            case EWarringPartiesType::White:
                oAttackingChessMans.Add(FAttackingChessMansData(nullptr, i));
                bCheckDirection = true;

            case EWarringPartiesType::NONE:
                lResult.Add(i);
                break;

            default:

                bCheckDirection = true;
                break;
            }

            // Выход из цикла
            if (bCheckDirection)
                break;
        }
    }

    return lResult;
}

TArray<FIntPoint> GetSquareForRook(
    const FIntPoint& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans)
{
    // Инициализация локальных переменных
    TArray<FIntPoint> lResult;
    FSquareData lSquareData; // Данные Клетки
    FIntPoint i;

    // Проверяемые направления для Ладьи
    TArray<FIntPoint> lCheckedDirections =
    {
        FIntPoint{ 1, 0 },
        FIntPoint { -1, 0 },
        FIntPoint { 0, 1 },
        FIntPoint { 0, -1 }
    };

    // Флаг выхода из цикла проверки конкретного направления
    bool bCheckDirection = false;

    // Выбор проверяемого направления по порядку
    for (auto& lDirection : lCheckedDirections)
    {
        bCheckDirection = false;

        // Проверка ходов Ладьи в текущем направлении
        for (i = iPosition + lDirection;
            CheckSquare(i, iAllSquares, lSquareData);
            i += lDirection)
        {
            switch (lSquareData.WarringPartiesType)
            {
            case EWarringPartiesType::White:
                oAttackingChessMans.Add(FAttackingChessMansData(nullptr, i));
                bCheckDirection = true;

            case EWarringPartiesType::NONE:
                lResult.Add(i);
                break;

            default:

                bCheckDirection = true;
                break;
            }

            // Выход из цикла
            if (bCheckDirection)
                break;
        }
    }

    return lResult;
}

TArray<FIntPoint> GetSquareForQueen(
    const FIntPoint& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans)
{
    // Инициализация локальных переменных
    TArray<FIntPoint> lResult;

    lResult = GetSquareForBishop(
        iPosition,
        iAllSquares,
        oAttackingChessMans);

    lResult += GetSquareForRook(
        iPosition,
        iAllSquares,
        oAttackingChessMans);

    return lResult;
}
//--------------------------------------------------------------------------------------



/* ---   Functions | Tools   --- */

void CheckPositions(
    const TArray<FIntPoint>& iNotChecked,
    const FSquareArray2D* iAllSquares,
    TArray<FIntPoint>& oResult,
    TArray<FAttackingChessMansData>& oAttackingChessMans)
{
    FSquareData lSquareData; // Данные Клетки

    for (auto& dPosition : iNotChecked)
    {
        if (CheckSquare(dPosition, iAllSquares, lSquareData))
        {
            switch (lSquareData.WarringPartiesType)
            {
            case EWarringPartiesType::White:
                oAttackingChessMans.Add(FAttackingChessMansData(nullptr, dPosition));
            case EWarringPartiesType::NONE:
                oResult.Add(dPosition);
                break;
            }
        }
    }
}

bool CheckSquare(
    const FIntPoint& iPosition,
    const FSquareArray2D* iAllSquares,
    FSquareData& oSquareData)
{
    if (iAllSquares->IsValidIndex(iPosition))
    {
        if (ASquare* lSquare = iAllSquares->GetByIndex(iPosition))
        {
            oSquareData = lSquare->GetData();
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------
