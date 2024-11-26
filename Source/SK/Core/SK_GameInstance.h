//

#pragma once

// Base:
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

// Structs:
#include "SK/Tools/Saving/LevelData.h"

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



    /* ---   Level Saving   --- */

    // Флаг новой игры. Если false, то игра будет зашружена из последнего сохранения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Level Saving")
    bool bIsNewGame = true;

    //

    /** Сохранение данных уровня (Отключено!) */
    void SaveLevelData(const FLevelData& LevelData) const;

    /** Получение сохранённых данных уровня (Отключено!) */
    FLevelData LoadLevelData() const;

    /** Проверка на наличие сохранения */
    UFUNCTION(BlueprintPure, Category = "Level Saving",
        meta = (CompactNodeTitle = "Is Empty?"))
    bool IsEmpty();
    //-------------------------------------------



private:

    /* ---   Level Saving   --- */

    // Наименование слотов сохранения данных уровня
    FString LevelDataSlot = "LevelData";

    // Сохраняемые данные уровня
    UPROPERTY()
    USavedLevelData* SaveLevel;

    //

    /** Инициализация сохранения данных уровня */
    void LevelSavingInit();

    /**	Обнуление данных уровня */
    void ClearLevelData();
    //-------------------------------------------
};
