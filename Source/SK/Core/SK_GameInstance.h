//

#pragma once

// Base:
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

// Structs:
#include "SK/Tools/Saving/Level/LevelData.h"
#include "SK/Tools/Saving/Level/LevelSelectionData.h"
#include "SK/InteractiveObjects/NoteStruct.h"

// Generated:
#include "SK_GameInstance.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Delegates   --- */

// Делегат информирования о сохранении текущего состояния Уровня
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelDataSave);
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Interaction:
class USaveLevel;
class USaveSettings;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API USK_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    /* ---   Delegates   --- */

    // Делегат информирования о сохранении текущего состояния Уровня
    UPROPERTY(BlueprintAssignable)
    FOnLevelDataSave OnLevelDataSave;
    //-------------------------------------------



    /* ---   Base   --- */

    /** Инициализация данных GameInstance */
    virtual void Init() override;
    //-------------------------------------------



    /* ---   Settings System | Saving   --- */

    // Сохраняемые данные Настроек
    UPROPERTY(BlueprintReadWrite,
        Category = "Settings System|Saving")
    USaveSettings* SaveSettings = nullptr;

    //

    // Получить частоту обновления Экрана монитора
    UFUNCTION(BlueprintCallable,
        Category = "Settings System")
    const int32 GetDisplayFrequency() const;
    //-------------------------------------------



    /* ---   Levels System | Saving   --- */

    // Флаг новой игры. Если false, то игра будет загружена из последнего сохранения (если оно есть)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
        Category = "Levels System|Saving")
    bool bIsNewGame = false;

    //

    /** Сохранение данных Уровня */
    void SaveLevelData(const FLevelData& LevelData) const;

    /**	Обнуление данных Уровня */
    UFUNCTION(BlueprintCallable,
        Category = "Levels System|Saving")
    void ClearLevelData();

    /** Получение сохранённых данных Уровня */
    FLevelData LoadLevelData() const;

    /** Получение сохранённых данных о выборе Уровня */
    FLevelSelectionData LoadLevelSelectionData() const;

    /** Проверка на наличие сохранения Уровня */
    UFUNCTION(BlueprintPure,
        Category = "Levels System|Saving",
        meta = (CompactNodeTitle = "Is Game Saved?"))
    bool IsGameSaved();
    //-------------------------------------------



    /* ---   Levels System | Selection   --- */

    // Таблица порядка прохождения сюжетных Уровней
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Levels System|Selection",
        meta = (RequiredAssetDataTags = "RowStructure=LevelTableRow"))
    UDataTable* StoryLevels = nullptr;

    // Карта Меню, запускаемая при неполадках "Level System" данного "Game Instance"
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Levels System|Selection")
    TSoftObjectPtr<UWorld> MenuMap;

    // Карта Хорошей концовки
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Levels System|Selection")
    TSoftObjectPtr<UWorld> MapGoodEnd;

    // Карта Плохой концовки
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Levels System|Selection")
    TSoftObjectPtr<UWorld> MapBadEnd;

    // Карта Плохой концовки
    UPROPERTY(BlueprintReadWrite,
        Category = "Levels System|Selection")
    bool bIsGoodEnd = false;

    //

    /** Продолжить ранее сохранённую игру */
    UFUNCTION(BlueprintCallable,
        Category = "Levels System|Selection")
    void ResumeGame();

    /** Начать новую игру (сохранённые данные игры обнулятся) */
    UFUNCTION(BlueprintCallable,
        Category = "Levels System|Selection")
    void NewGame();

    /** Переход к следующему Уровню */
    UFUNCTION(BlueprintCallable,
        Category = "Levels System|Selection")
    void NextLevel();

    /** Переход к следующему Уровню */
    UFUNCTION(BlueprintCallable,
        Category = "Levels System|Selection")
    void LevelRestart();
    //-------------------------------------------



    /* ---   Levels System | Intermediate   --- */

    // Позиция Игрока при переходе между Уровнями
    UPROPERTY(BlueprintReadWrite,
        Category = "Levels System|Selection")
    int32 IntermediatePlayerPositionY = -1;

    // Текущее Смещение позиций Игрока по оси Y при переходе с предыдущей карты
    int32 CurrentOffsetPlayersPositionAlongY = 0;
    //-------------------------------------------



    /* ---   Note System   --- */

    // Таблица Записок
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Note System")
    FNotesData NotesData;

    //

    /** Событие Взятия записки */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "Note System",
        meta = (DisplayName = "Taking Note"))
    void EventTakingNote(FName NoteName);

    /** "Взятие" записки с проверкой по наименованию строки в соответствующим DataTable */
    void TakingNote(const FName& NoteName);

    /** Проверка на наличие данной записки */
    bool CheckNote(const FName& NoteName) const;

    /** Получить собранные Записки */
    UFUNCTION(BlueprintCallable,
        Category = "Note System")
    TMap<int32, FNotesTableRow> GetCollectedNotes() const;
    //-------------------------------------------



private:

    /* ---   Settings System | Saving   --- */

    // Наименование слотов сохранения данных Настроек
    FString SettingsDataSlot = "SettingsData";

    //

    /** Инициализация сохранения данных Настроек */
    void SettingsSavingInit();
    //-------------------------------------------



    /* ---   Levels System | Saving   --- */

    // Наименование слотов сохранения данных Уровня
    FString LevelDataSlot = "LevelData";

    // Сохраняемые данные Уровня
    UPROPERTY()
    USaveLevel* SaveLevel;

    //

    /** Инициализация сохранения данных Уровня */
    void LevelSavingInit();
    //-------------------------------------------



    /* ---   Levels System | Selection   --- */

    // Текущий номер Уровня (строки из "StoryLevels")
    int32 NextLevelNumber = 0;
    //-------------------------------------------



    /* ---   Note System   --- */

    void NotesDataInit();
    //-------------------------------------------
};
