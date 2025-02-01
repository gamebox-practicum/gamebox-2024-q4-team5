//

#pragma once

// Base:
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

// Structs:
#include "SK/Tools/Saving/LevelData.h"
#include "SK/Tools/Saving/LevelSelectionData.h"

// Generated:
#include "SK_GameInstance.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Delegates   --- */

// Делегат информирования о сохранении текущего состояния уровня
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelDataSave);
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Interaction:
class USavedLevelData;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API USK_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    /* ---   Delegates   --- */

    // Делегат информирования о сохранении текущего состояния уровня
    UPROPERTY(BlueprintAssignable)
    FOnLevelDataSave OnLevelDataSave;
    //-------------------------------------------



    /* ---   Base   --- */

    /** Инициализация данных GameInstance */
    virtual void Init() override;
    //-------------------------------------------



    /* ---   Levels System | Saving   --- */

    // Флаг новой игры. Если false, то игра будет загружена из последнего сохранения (если оно есть)
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Levels System|Saving")
    bool bIsNewGame = false;

    //

    /** Сохранение данных уровня (Отключено!) */
    void SaveLevelData(const FLevelData& LevelData) const;

    /**	Обнуление данных уровня */
    UFUNCTION(BlueprintCallable,
        Category = "Levels System|Saving")
    void ClearLevelData();

    /** Получение сохранённых данных уровня */
    FLevelData LoadLevelData() const;

    /** Получение сохранённых данных о выборе уровня */
    FLevelSelectionData LoadLevelSelectionData() const;

    /** Проверка на наличие сохранения */
    UFUNCTION(BlueprintPure,
        Category = "Levels System|Saving",
        meta = (CompactNodeTitle = "Is Game Saved?"))
    bool IsGameSaved();
    //-------------------------------------------



    /* ---   Levels System | Selection   --- */

    // Таблица порядка прохождения сюжетных уровней
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

    /** Переход к следующему уровню */
    UFUNCTION(BlueprintCallable,
        Category = "Levels System|Selection")
    void NextLevel();

    /** Переход к следующему уровню */
    UFUNCTION(BlueprintCallable,
        Category = "Levels System|Selection")
    void LevelRestart();
    //-------------------------------------------



    /* ---   Levels System | Intermediate   --- */

    // Позиция Игрока при переходе между уровнями
    UPROPERTY(BlueprintReadWrite,
        Category = "Levels System|Selection")
    int32 IntermediatePlayerPositionY = -1;

    // Текущее Смещение позиций Игрока по оси Y при переходе с предыдущей карты
    int32 CurrentOffsetPlayersPositionAlongY = 0;
    //-------------------------------------------



private:

    /* ---   Levels System | Saving   --- */

    // Наименование слотов сохранения данных уровня
    FString LevelDataSlot = "LevelData";

    // Сохраняемые данные уровня
    UPROPERTY()
    USavedLevelData* SaveLevel;

    //

    /** Инициализация сохранения данных уровня */
    void LevelSavingInit();

    //-------------------------------------------



    /* ---   Levels System | Selection   --- */

    // Текущий номер уровня (строки из "StoryLevels")
    int32 NextLevelNumber = 0;
    //-------------------------------------------
};
