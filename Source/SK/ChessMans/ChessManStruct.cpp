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
        TArray<FIndex2D> lSquaresIndex2D;

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

            // Если были добавлены позиции аттаки, то заполнить указатели на текущую Шахматную фигуру
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

TArray<FIndex2D> GetSquareForPawn(
    const FIndex2D& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans)
{
    // Инициализация локальных переменных
    TArray<FIndex2D> lNotChecked;
    TArray<FIndex2D> lResult;
    FSquareData lSquareData; // Данные Клетки

    // Ход вперёд
    lNotChecked.Add(iPosition + FIndex2D { -1, 0 });

    // Проверка короткого и длинного хода вперёд
    CheckPositions(
        lNotChecked,
        iAllSquares,
        lResult,
        oAttackingChessMans);

    // Сброс переменной
    lNotChecked.Empty(2);

    // Ход по диагонали
    lNotChecked.Add(iPosition + FIndex2D { -1, 1 });
    lNotChecked.Add(iPosition + FIndex2D { -1, -1 });

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

TArray<FIndex2D> GetSquareForKnight(
    const FIndex2D& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans)
{
    // Инициализация локальных переменных
    TArray<FIndex2D> lNotChecked;
    lNotChecked.Reset(8);
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
    CheckPositions(
        lNotChecked,
        iAllSquares,
        lResult,
        oAttackingChessMans);

    return lResult;
}

TArray<FIndex2D> GetSquareForBishop(
    const FIndex2D& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans)
{
    // Инициализация локальных переменных
    TArray<FIndex2D> lResult;
    FSquareData lSquareData; // Данные Клетки
    FIndex2D i;

    // Проверяемые направления для Слона
    TArray<FIndex2D> lCheckedDirections =
    {
        FIndex2D{ 1, 1 },
        FIndex2D { 1, -1 },
        FIndex2D { -1, -1 },
        FIndex2D { -1, 1 }
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

TArray<FIndex2D> GetSquareForRook(
    const FIndex2D& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans)
{
    // Инициализация локальных переменных
    TArray<FIndex2D> lResult;
    FSquareData lSquareData; // Данные Клетки
    FIndex2D i;

    // Проверяемые направления для Ладьи
    TArray<FIndex2D> lCheckedDirections =
    {
        FIndex2D{ 1, 0 },
        FIndex2D { -1, 0 },
        FIndex2D { 0, 1 },
        FIndex2D { 0, -1 }
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

TArray<FIndex2D> GetSquareForQueen(
    const FIndex2D& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans)
{
    // Инициализация локальных переменных
    TArray<FIndex2D> lResult;

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
    const TArray<FIndex2D>& iNotChecked,
    const FSquareArray2D* iAllSquares,
    TArray<FIndex2D>& oResult,
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
    const FIndex2D& iPosition,
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
