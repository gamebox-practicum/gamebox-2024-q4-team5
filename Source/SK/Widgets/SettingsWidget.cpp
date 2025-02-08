//

// Base:
#include "SettingsWidget.h"

// UE:
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"

// Interaction:
#include "SK/Core/SK_GameInstance.h"
#include "SK/Tools/Saving/Settings/SaveSettings.h"
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void USettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InitWidgetData();
}

void USettingsWidget::NativeConstruct()
{
    Super::NativeConstruct();
}
//--------------------------------------------------------------------------------------



/* ---   Settings System | Saving   --- */

void USettingsWidget::LoadSettings()
{
    if (CurrentSavedSettings != SaveSettings->SettingsData)
    {
        SaveSettings = Cast<USaveSettings>(UGameplayStatics::LoadGameFromSlot(SettingsDataSlot, 0));
        CurrentSavedSettings = SaveSettings->SettingsData;
    }

    if (bIsVideoSettingsDirty
        || GameUserSettings->IsDirty())
    {
        GameUserSettings->LoadSettings();
        bIsVideoSettingsDirty = false;
    }
}

void USettingsWidget::ApplySettings()
{
    if (CurrentSavedSettings != SaveSettings->SettingsData)
    {
        UGameplayStatics::SaveGameToSlot(SaveSettings, SettingsDataSlot, 0);
        CurrentSavedSettings = SaveSettings->SettingsData;
    }

    if (bIsVideoSettingsDirty
        || GameUserSettings->IsDirty())
    {
        GameUserSettings->ValidateSettings();
        GameUserSettings->ApplyHardwareBenchmarkResults();
        GameUserSettings->ApplySettings(false);
        bIsVideoSettingsDirty = false;
    }
}

bool USettingsWidget::IsSettingsDirty()
{
    return bIsVideoSettingsDirty
        || CurrentSavedSettings != SaveSettings->SettingsData
        || GEngine->GetGameUserSettings()->IsDirty();
}

void USettingsWidget::InitWidgetData()
{
    SaveSettings = Cast<USK_GameInstance>(GetGameInstance())->SaveSettings;
    CurrentSavedSettings = SaveSettings->SettingsData;
    GameUserSettings = GEngine->GetGameUserSettings();
}
//--------------------------------------------------------------------------------------
