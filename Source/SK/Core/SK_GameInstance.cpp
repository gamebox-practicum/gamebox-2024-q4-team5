//

// Base:
#include "SK_GameInstance.h"

// Win:
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"
#include <windows.h> // Conflict header file
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif

// UE:
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

// Interaction:
#include "SK/Tools/Saving/Level/SaveLevel.h"
#include "SK/Tools/Saving/Settings/SaveSettings.h"
#include "SK/InteractiveObjects/InteractiveNote.h"
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void USK_GameInstance::Init()
{
    SettingsSavingInit();
    NotesDataInit();
    LevelSavingInit();

    Super::Init();
}
//--------------------------------------------------------------------------------------



/* ---   Settings System | Saving   --- */

const int32 USK_GameInstance::GetDisplayFrequency() const
{
    int32 lFPS = 60;

    // Ограничение по платформе: Только WINDOWS
#if PLATFORM_WINDOWS
    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
    lFPS = dm.dmDisplayFrequency;
#endif

    return lFPS;
}

void USK_GameInstance::SettingsSavingInit()
{
    SaveSettings = Cast<USaveSettings>(UGameplayStatics::LoadGameFromSlot(SettingsDataSlot, 0));

    if (!SaveSettings)
    {
        UGameUserSettings* lGameUserSettings = GEngine->GetGameUserSettings();

        /* ---   Auto settings:   --- */
        // Автоматическое определение, принятие и сохранение всех Настроек Видео:
        lGameUserSettings->RunHardwareBenchmark();          // Определение
        lGameUserSettings->ApplyHardwareBenchmarkResults(); // Принятие и Сохранение
        //-------------------------------------------


        /* ---   Default settings:   --- */

        // Полный экран
        lGameUserSettings->SetFullscreenMode(EWindowMode::Fullscreen);

        // Разрешение экрана
        TArray<FIntPoint> lAllRes; // Все доступные вариации Разрешения экрана (от меньшего к большему)
        UKismetSystemLibrary::GetSupportedFullscreenResolutions(lAllRes);
        // Принятие последнего (наибольшего) значения Разрешения экрана
        lGameUserSettings->SetScreenResolution(lAllRes[lAllRes.Num() - 1]);

        // FPS
        lGameUserSettings->SetFrameRateLimit(GetDisplayFrequency());


        lGameUserSettings->ApplySettings(false);
        //-------------------------------------------


        // Создание и сохранение базовых Настроек: Игровой процесс, Звук
        SaveSettings = Cast<USaveSettings>(UGameplayStatics::CreateSaveGameObject(
            USaveSettings::StaticClass()));

        UGameplayStatics::SaveGameToSlot(SaveSettings, SettingsDataSlot, 0);
    }

    // PS: Так как данная функция вызывается в Init(), SaveSettings будет валиден всегда
}
//--------------------------------------------------------------------------------------



/* ---   Levels System | Saving   --- */

void USK_GameInstance::SaveLevelData(const FLevelData& iLevelData) const
{
    if (SaveLevel)
    {
        // Данные об уровне
        SaveLevel->LevelSelectionData =
        {
            StoryLevels,
            NextLevelNumber
        };

        // Данные уровня
        SaveLevel->LevelData = iLevelData;

        // Данные системы Записок 
        SaveLevel->NotesData = NotesData;

        UGameplayStatics::SaveGameToSlot(SaveLevel, LevelDataSlot, 0);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s'::SaveLevelData: SaveLevel is NOT"),
            *GetNameSafe(this));
    }
}

void USK_GameInstance::ClearLevelData()
{
    if (IsGameSaved())
    {
        // Данные об уровне
        SaveLevel->LevelSelectionData = FLevelSelectionData::Empty;

        // Данные уровня
        SaveLevel->LevelData = FLevelData::Empty;

        // Данные системы Записок 
        SaveLevel->NotesData = FNotesData::Empty;

        UGameplayStatics::SaveGameToSlot(SaveLevel, LevelDataSlot, 0);
    }
}

FLevelData USK_GameInstance::LoadLevelData() const
{
    if (SaveLevel)
    {
        return SaveLevel->LevelData;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s'::LoadLevelData: SaveLevel is NOT"),
            *GetNameSafe(this));
    }

    return FLevelData::Empty;
}

FLevelSelectionData USK_GameInstance::LoadLevelSelectionData() const
{
    if (SaveLevel)
    {
        return SaveLevel->LevelSelectionData;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s'::LoadLevelSelectionData: SaveLevel is NOT"),
            *GetNameSafe(this));
    }

    return FLevelSelectionData::Empty;
}

bool USK_GameInstance::IsGameSaved()
{
    if (SaveLevel)
    {
        return SaveLevel->LevelData.PlayersData.Num() != 0;
    }

    return false;
}

void USK_GameInstance::LevelSavingInit()
{
    SaveLevel = Cast<USaveLevel>(UGameplayStatics::LoadGameFromSlot(LevelDataSlot, 0));

    if (SaveLevel)
    {
        if (SaveLevel->NotesData.NotesTable
            && SaveLevel->NotesData.NotesChecker.Num() > 0)
        {
            NotesData = SaveLevel->NotesData;
        }
    }
    else
    {
        SaveLevel = Cast<USaveLevel>(UGameplayStatics::CreateSaveGameObject(
            USaveLevel::StaticClass()));

        ClearLevelData();
    }
}
//--------------------------------------------------------------------------------------



/* ---   Levels System | Selection   --- */

void USK_GameInstance::ResumeGame()
{
    FLevelSelectionData lLevelSelectionData = LoadLevelSelectionData();
    StoryLevels = lLevelSelectionData.LevelsTable;
    NextLevelNumber = lLevelSelectionData.LevelNumber - 1;

    NextLevel();
}

void USK_GameInstance::NewGame()
{
    bIsNewGame = true;
    NextLevelNumber = 0;
    IntermediatePlayerPositionY = -1;

    NotesDataInit();

    NextLevel();
}

void USK_GameInstance::NextLevel()
{
    if (StoryLevels)
    {
        // Массив данных, получаемых из DataTable
        TArray<FLevelTableRow*> lLevels;
        // Карта, получаемая из Массива
        TSoftObjectPtr<UWorld> lNewLevel;

        StoryLevels->GetAllRows("USK_GameInstance::NextLevel", lLevels);

        while (!lNewLevel.GetAssetName().Len() && NextLevelNumber < lLevels.Num())
        {
            lNewLevel = lLevels[NextLevelNumber]->Map;
            CurrentOffsetPlayersPositionAlongY = lLevels[NextLevelNumber]->OffsetPlayersPositionAlongY;

            ++NextLevelNumber;
        };

        if (lNewLevel.GetAssetName().Len())
        {
            UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), lNewLevel);
        }
        else
        {
            if (bIsGoodEnd && MapGoodEnd.GetAssetName().Len())
            {
                UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MapGoodEnd);
            }
            else if (!bIsGoodEnd && MapBadEnd.GetAssetName().Len())
            {
                UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MapBadEnd);
            }
            else if (MenuMap.GetAssetName().Len())
            {
                UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MenuMap);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s'::NextLevel: StoryLevels is NOT"),
            *GetNameSafe(this));
    }
}

void USK_GameInstance::LevelRestart()
{
    --NextLevelNumber;

    NextLevel();
}
//--------------------------------------------------------------------------------------



/* ---   Note System   --- */

void USK_GameInstance::TakingNote(const FName& NoteName)
{
    if (NotesData.NotesTable)
    {
        int32 lInd = NotesData.NotesTable->GetRowNames().IndexOfByKey(NoteName);

        if (lInd != INDEX_NONE)
        {
            NotesData.NotesChecker[lInd] = true;

            EventTakingNote(NoteName);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s'::TakingNote: NotesTable is NOT"),
            *GetNameSafe(this));
    }
}

bool USK_GameInstance::CheckNote(const FName& NoteName) const
{
    if (NotesData.NotesTable)
    {
        int32 lInd = NotesData.NotesTable->GetRowNames().IndexOfByKey(NoteName);

        if (lInd != INDEX_NONE)
        {
            return NotesData.NotesChecker[lInd] == true;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s'::CheckNote: NotesTable is NOT"),
            *GetNameSafe(this));
    }

    return false;
}

TMap<int32, FNotesTableRow> USK_GameInstance::GetCollectedNotes() const
{
    TMap<int32, FNotesTableRow> lNotesData;
    TArray<FNotesTableRow*> lRows;

    NotesData.NotesTable->GetAllRows<FNotesTableRow>("USK_GameInstance::GetCollectedNotes()", lRows);

    for (int32 i = 0;
        i < NotesData.NotesChecker.Num();
        ++i)
    {
        if (NotesData.NotesChecker[i] == true)
        {
            lNotesData.Add(i + 1, *lRows[i]);
        }
    }

    return lNotesData;
}

void USK_GameInstance::NotesDataInit()
{
    // Инициализация и присвоение false
    NotesData.NotesChecker.Init(false, NotesData.NotesTable ? NotesData.NotesTable->GetRowNames().Num() : 0);
}
//--------------------------------------------------------------------------------------
