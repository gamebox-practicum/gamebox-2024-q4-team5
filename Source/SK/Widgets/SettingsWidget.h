#pragma once

// Base:
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Structs:
#include "SK/Tools/Saving/Settings/SettingsData.h"

// Generated:
#include "SettingsWidget.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Interaction:
class USaveSettings;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API USettingsWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    /* ---   Base   --- */

    /** Аналогичен OnInitialized() */
    virtual void NativeOnInitialized() override;

    /** Аналогичен Construct() */
    virtual void NativeConstruct() override;
    //--------------------------------------------



    /* ---   Settings System | Saving   --- */

    // Флаг контроля изменений Настроек Видео
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
        Category = "Settings System|Saving",
        meta = (DisplayName = "Is Video Settings Dirty?"))
    bool bIsVideoSettingsDirty = false;

    // Сохраняемые данные Настроек
    UPROPERTY(BlueprintReadWrite,
        Category = "Settings System|Saving")
    USaveSettings* SaveSettings = nullptr;

    //

    /** Выгрузить Настройки */
    UFUNCTION(BlueprintCallable,
        Category = "Settings System|Saving")
    void LoadSettings();

    /** Сохранение Настроек */
    UFUNCTION(BlueprintCallable,
        Category = "Settings System|Saving")
    void ApplySettings();

    /** Определить изменены ли настройки Настройки */
    UFUNCTION(BlueprintPure,
        Category = "Settings System|Saving",
        meta = (CompactNodeTitle = "Is Settings Dirty?"))
    bool IsSettingsDirty();
    //-------------------------------------------



private:

    /* ---   Settings System | Saving   --- */

    // Текущий GameUserSettings
    UGameUserSettings* GameUserSettings = nullptr;

    // Наименование слотов сохранения данных Настроек
    FString SettingsDataSlot = "SettingsData";

    // "Заместитель" для сохранённых Настроек
    FSettingsData CurrentSettingsData;

    //

    // Инициализация данных
    void InitWidgetData();
    //-------------------------------------------
};
