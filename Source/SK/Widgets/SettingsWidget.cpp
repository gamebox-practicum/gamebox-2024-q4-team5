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
    InitWidgetData();

    Super::NativeOnInitialized();
}

void USettingsWidget::NativeConstruct()
{
    Super::NativeConstruct();
}
//--------------------------------------------------------------------------------------



/* ---   Settings System | Saving   --- */

void USettingsWidget::LoadSettings()
{
    if (CurrentSettingsData != SaveSettings->SettingsData)
    {
        SaveSettings->SettingsData = CurrentSettingsData;
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
    if (CurrentSettingsData != SaveSettings->SettingsData)
    {
        UGameplayStatics::SaveGameToSlot(SaveSettings, SettingsDataSlot, 0);
        CurrentSettingsData = SaveSettings->SettingsData;
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
        || CurrentSettingsData != SaveSettings->SettingsData
        || GEngine->GetGameUserSettings()->IsDirty();
}

void USettingsWidget::InitWidgetData()
{
    SaveSettings = Cast<USK_GameInstance>(GetGameInstance())->SaveSettings;
    CurrentSettingsData = SaveSettings->SettingsData;
    GameUserSettings = GEngine->GetGameUserSettings();
}
//--------------------------------------------------------------------------------------
