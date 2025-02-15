// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Structs:
#include "NoteStruct.h"

// Generated:
#include "InteractiveNote.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Interaction:
class USK_GameInstance;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API AInteractiveNote : public AActor
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    AInteractiveNote();
    //-------------------------------------------



    /* ---   Components   --- */

    /** Меш визуализации блока */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* NoteMesh = nullptr;
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Interaction   --- */

    /** Событие, когда на него наводят курсор мыши с помощью интерактивного интерфейса */
    virtual void NotifyActorBeginCursorOver() override;

    /** Событие, когда с него убирают курсор мыши с помощью интерактивного интерфейса */
    virtual void NotifyActorEndCursorOver() override;

    /** Событие, когда этот субъект находится под мышкой при нажатии клавиши реагирования с помощью интерактивного интерфейса */
    virtual void NotifyActorOnClicked(FKey ButtonReleased) override;
    //-------------------------------------------



    /* ---   Note System   --- */

    // Таблица Записок
    // @note    Используется для отображения списка вариантов в "NoteAuthor"
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Note System",
        meta = (RequiredAssetDataTags = "RowStructure=NotesTableRow"))
    UDataTable* NotesTable = nullptr;

    // Текущий выбранный Автор Записки
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Note System",
        meta = (GetOptions = "GetAuthorOptions"))
    FName NoteType = "NONE";

    //-------------------------------------------



private:

    /* ---   Note System   --- */

    // Указатель на текущий USK_GameInstance
    USK_GameInstance* CurrentGameInstance = nullptr;

    //

    /** Предварительная инициализация Записки */
    void InitNoteData();

    /** Получение всех Авторов Записок (наименования строк) */
    UFUNCTION()
    TArray<FName> GetAuthorOptions() const;
    //-------------------------------------------
};
