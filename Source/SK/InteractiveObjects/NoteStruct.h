#pragma once

// Base:
#include "CoreMinimal.h"

// UE:
#include "Engine/DataTable.h"
#include "Containers/StaticBitArray.h"
// Generated:
#include "NoteStruct.generated.h"
//--------------------------------------------------------------------------------------



// Структура строки Таблицы данных Записок
USTRUCT(BlueprintType)
struct FNotesTableRow : public FTableRowBase
{
    GENERATED_BODY()

    // Заголовок записки
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Header = {};

    // Текст записки
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        meta = (MultiLine = true))
    FText Text = {};
    //-------------------------------------------


    /* ---   Constructors   --- */

    FNotesTableRow() {};
    //-------------------------------------------
};



// Структура данных Записок
USTRUCT(BlueprintType)
struct FNotesData
{
    GENERATED_BODY()

    /* ---   Notes System Data   --- */

    // Таблица Записок
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        meta = (RequiredAssetDataTags = "RowStructure=NotesTableRow"))
    UDataTable* NotesTable = nullptr;

    // Контейнер флагов, контролирующих взятие соответствующих им записок
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<bool> NotesChecker;
    /* PS:
    1) Сохраняются переменные только со Спецификатором "UPROPERTY()";
    2) Компактные контейнеры не поддерживают "UPROPERTY()" и, следовательно, не могут быть сохранены.
        Например:
        * std::vector<bool>;
        * TBitArray<>;
        * TStaticBitArray<>;
    3) Так как "Записок" довольно мало (на момент написания было 8 шт.), то "компактность" не критична.
    */

    //-------------------------------------------


    /* ---   Static   --- */

    // Пустой: Все значения структуры равны базовыми значениям
    static const FNotesData Empty;
    //-------------------------------------------
};



/* ---   Переопределение для статических значений структуры   --- */

// Пустой: Все значения структуры = 0
__declspec(selectany) const FNotesData FNotesData::Empty
{
    /* ---   Notes System Data   --- */
    nullptr, // Таблица Записок
    TArray<bool>(), // Контейнер флагов
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
