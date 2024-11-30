#pragma once

// Base:
#include "CoreMinimal.h"

// Tools:
#include "SK/Tools/Index2D.h"

// UE:
#include "Engine/DataTable.h"

// Structs:
#include "SK/ChessBoard/SquareStruct.h"

// Generated:
#include "ChessManStruct.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// Interaction:
class ASK_Character;
class AChessMan;
class ASquare;
//--------------------------------------------------------------------------------------



/* ---   Data   --- */

// Тип Фигуры
UENUM(BlueprintType)
enum struct EChessManType : uint8
{
    // Пешка
    Pawn,
    // Конь
    Knight,
    // Слон
    Bishop,
    // Ладья
    Rook,
    // Ферзь (Королева)
    Queen,
    // Король
    King UMETA(Hidden),

    // Базовый без эффектов
    NONE UMETA(Hidden)// Всегда в конце для строгого назначения количества элементов массива
};



// Тип направления ротации Шахматной фигуры
UENUM(BlueprintType)
enum struct EChessManRotationType : uint8
{
    // Конь
    Knight,
    // Слон
    Bishop,
    // Ладья
    Rook,
    // Ферзь (Королева)
    Queen,

    // В сторону игрока
    ToCharacter,

    // Базовый без эффектов
    NONE
};



// Данные Игроков
USTRUCT(BlueprintType)
struct FPlayerData : public FTableRowBase
{
    GENERATED_BODY()

    // Тип Фигуры
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ASK_Character> Type;

    // Позиция на доске
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIndex2D Position = { 0,0 };
    //-------------------------------------------



    /* ---   Constructors   --- */

    FPlayerData() {};

    FPlayerData(TSubclassOf<ASK_Character> iType, FIndex2D iPosition)
        : Type(iType), Position(iPosition) {};
    //-------------------------------------------
};



// Данные Фигуры
USTRUCT(BlueprintType)
struct FChessManData : public FTableRowBase
{
    GENERATED_BODY()

    // Тип Фигуры
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EChessManType Type = EChessManType::NONE;

    // Позиция на доске
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIndex2D Position = { 0,0 };

    // Возможные позиции для хода
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FIndex2D> AvailablePositions;

    // Флаг состояния смерти данной Шахматной фигуры
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsDead = false;
    //-------------------------------------------



    /* ---   Constructors   --- */

    FChessManData() {};
    //-------------------------------------------
};



// Данные об Атакующих Шахматных фигурах
USTRUCT(BlueprintType)
struct FAttackingChessMansData
{
    GENERATED_BODY()

    // Тип Фигуры
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AChessMan* ChessMan = nullptr;

    // Позиция на доске
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIndex2D AttackPosition = { 0,0 };
    //-------------------------------------------



    /* ---   Constructors   --- */

    FAttackingChessMansData() {};

    FAttackingChessMansData(AChessMan* iChessMan, const FIndex2D& iAttackPosition)
        : ChessMan(iChessMan), AttackPosition(iAttackPosition) {};
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------



/* ---   Functions   --- */

/** Обновление данных о доступных ходах
@param  iChessMan   -- Указатель на проверяемую Шахматную фигуру
@param  iAllSquares -- Указатель на двумерный Массив Клеток

@return Если указатель iChessMan имеет доступный ход, то возвращает указатель iChessMan.
В противном случае -- nullptr
*/
AChessMan* GetAvailableChessMan(
    AChessMan* ChessMan,
    const FSquareArray2D* AllSquares,
    TArray<FAttackingChessMansData>& AttackingChessMans);
//--------------------------------------------------------------------------------------



/* ---   Functions | Get Square For ChessMan   --- */

/** Получить индексы всех Клеток для данной Пешки
@param  iPosition   -- Текущая позиция Пешки
@param  iAllSquares -- Указатель на двумерный Массив Клеток

@return Массив свободных индексов всех Клеток для данной Пешки.
*/
TArray<FIndex2D> GetSquareForPawn(
    const FIndex2D& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans);

/** Получить индексы всех Клеток для данного Коня
@param  iPosition   -- Текущая позиция Коня
@param  iAllSquares -- Указатель на двумерный Массив Клеток

@return Массив свободных индексов всех Клеток для данного Коня
*/
TArray<FIndex2D> GetSquareForKnight(
    const FIndex2D& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans);

/** Получить индексы всех Клеток для данного Слона
@param  iPosition   -- Текущая позиция Слона
@param  iAllSquares -- Указатель на двумерный Массив Клеток

@return Массив свободных индексов всех Клеток для данного Слона
*/
TArray<FIndex2D> GetSquareForBishop(
    const FIndex2D& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans);

/** Получить индексы всех Клеток для данного Ладьи
@param  iPosition   -- Текущая позиция Ладьи
@param  iAllSquares -- Указатель на двумерный Массив Клеток

@return Массив свободных индексов всех Клеток для данного Ладьи
*/
TArray<FIndex2D> GetSquareForRook(
    const FIndex2D& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans);

/** Получить индексы всех Клеток для данного Ферзя
@param  iPosition   -- Текущая позиция Ферзя
@param  iAllSquares -- Указатель на двумерный Массив Клеток

@return Массив свободных индексов всех Клеток для данного Ферзя
*/
TArray<FIndex2D> GetSquareForQueen(
    const FIndex2D& iPosition,
    const FSquareArray2D* iAllSquares,
    TArray<FAttackingChessMansData>& oAttackingChessMans);
//--------------------------------------------------------------------------------------



/* ---   Functions | Tools   --- */

/** Проверка наличия Клетки по индексу
@param  iPosition   -- Текущая позиция Пешки
@param  iNotChecked -- Проверяемый массив 2D-индексов Клеток
@param  iAllSquares -- Указатель на двумерный Массив Клеток

@return oResult     -- Проверенный массив 2D-индексов свободных Клеток
*/
void CheckPositions(
    const TArray<FIndex2D>& iNotChecked,
    const FSquareArray2D* iAllSquares,
    TArray<FIndex2D>& oResult,
    TArray<FAttackingChessMansData>& oAttackingChessMans);

/** Проверка наличия Клетки по индексу
@param  iPosition   -- Текущая позиция Пешки
@param  iAllSquares -- Указатель на двумерный Массив Клеток

@return true        -- если Клетка в наличии
@return oSquareData -- Данные Клетки
*/
bool CheckSquare(
    const FIndex2D& iPosition,
    const FSquareArray2D* iAllSquares,
    FSquareData& oSquareData);
//--------------------------------------------------------------------------------------
