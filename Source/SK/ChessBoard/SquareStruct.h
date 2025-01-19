#pragma once

// Base:
#include "CoreMinimal.h"

// Tools:
#include "SK/Tools/Index2D.h"

// Generated:
#include "SquareStruct.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// Interaction:
class ASquare;
//--------------------------------------------------------------------------------------



// Тип враждующих сторон
UENUM(BlueprintType)
enum struct EWarringPartiesType : uint8
{
    // Свободный
    NONE = 0,
    // Белый
    White = 1,
    // Чёрный
    Black,
    // Труп Шахматной фигуры
    Corpse,
};



// Данные Клетки
USTRUCT(BlueprintType)
struct FSquareData
{
    GENERATED_BODY()

    // Позиционный номер
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIndex2D PositionNumber = { 0,0 };

    // Тип материала блока (номер по порядку: A=0, B=1)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool TypeBlockMaterial = 0;

    // Тип стороны конфликта
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWarringPartiesType WarringPartiesType = EWarringPartiesType::NONE;
    //-------------------------------------------



    /* ---   Constructors   --- */

    FSquareData() {};
    //-------------------------------------------
};



// Массив указателей на Клетки
USTRUCT(BlueprintType)
struct FSquareArray
{
    // PS: Необходим для создания двумерного массива.
    // Не получилось преобразовать в шаблон, который бы поддерживался в UE

    GENERATED_BODY()

    friend struct FSquareArray2D;
    //-------------------------------------------



    /* ---   Constructors   --- */

    FSquareArray() {};
    //-------------------------------------------



private:

    /* ---   Data   --- */

    // Ось Y
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly,
        meta = (AllowPrivateAccess = true, DisplayName = "Y"))
    TArray<ASquare*> SquareArray;
    //-------------------------------------------
};



// 2D-массив указателей на Клетки
USTRUCT(BlueprintType)
struct FSquareArray2D
{
    // PS: Необходим для создания двумерного массива.
    // Не получилось преобразовать в шаблон, который бы поддерживался в UE

    GENERATED_BODY()
    //-------------------------------------------



    /* ---   Constructors   --- */

    FSquareArray2D() {};
    //-------------------------------------------



    /* ---   Functions   --- */

    /** Проверка валидности индекса массива */
    bool IsValidIndex(const int32& X, const int32& Y) const;
    /** Проверка валидности индекса массива */
    bool IsValidIndex(const FIndex2D& XY) const;

    /** Очистка массива  */
    void Empty();
    /** Проверка на пустой ли массив  */
    bool IsEmpty() const;

    /** Изменить размер массива на заданное количество элементов */
    void SetNum(const int32& X, const int32& Y);
    /** Изменить размер массива на заданное количество элементов */
    void SetNum(const FIndex2D& XY);

    /** Изменить размер массива на заданное количество элементов */
    FIndex2D Num() const;

    /** Получить массив с конкретным индексом X */
    TArray<ASquare*> GetArrayByX(const int32& X) const;
    /** Получить массив с конкретным индексом Y */
    TArray<ASquare*> GetArrayByY(const int32& Y) const;

    /** Получить элемент по Индексу */
    ASquare* GetByIndex(const int32& X, const int32& Y) const;
    /** Получить элемент по Двумерному Индексу */
    ASquare* GetByIndex(const FIndex2D& XY) const;

    /** Записать элемент по Индексу */
    bool SetByIndex(ASquare* Square, const int32& X, const int32& Y);
    /** Записать элемент по Двумерному Индексу */
    bool SetByIndex(ASquare* Square, const FIndex2D& XY);
    //-------------------------------------------



private:

    /* ---   Data   --- */

    // Ось X
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly,
        meta = (AllowPrivateAccess = true, DisplayName = "X"))
    TArray<FSquareArray> Square2DArray;
    //-------------------------------------------
};
