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



/* ---   Pre-declaration of classes   --- */

// Interaction:
class USavedLevelData;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API USK_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    /* ---   Base   --- */

    /** Инициализация данных GameInstance */
    virtual void Init() override;
    //-------------------------------------------



    /* ---   Level Saving   --- */

    // Флаг новой игры. Если false, то игра будет зашружена из последнего сохранения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Level Saving")
    bool bIsNewGame = true;

    //

    /** Сохранение данных уровня */
    void SaveLevelData(const FLevelData& LevelData) const;

    /** Получение сохранённых данных уровня */
    FLevelData LoadLevelData() const;
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
