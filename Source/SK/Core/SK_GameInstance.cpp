//

// Base:
#include "SK_GameInstance.h"

// UE:
#include "Kismet/GameplayStatics.h"

// Interaction:
#include "SK/Tools/Saving/SavedLevelData.h"
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void USK_GameInstance::Init()
{
    Super::Init();

    LevelSavingInit();
}
//--------------------------------------------------------------------------------------



/* ---   Levels System | Saving   --- */

void USK_GameInstance::SaveLevelData(const FLevelData& iLevelData) const
{
    if (SaveLevel)
    {
        // Сбор данных об уровне
        FLevelSelectionData lLevelSelectionData;
        lLevelSelectionData.LevelsTable = StoryLevels;
        lLevelSelectionData.LevelNumber = NextLevelNumber;

        // Данные об уровне
        SaveLevel->LevelSelectionData = lLevelSelectionData;

        // Данные уровня
        SaveLevel->LevelData = iLevelData;

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
        UE_LOG(LogTemp, Warning, TEXT("'%s'::LoadLevelData: SaveLevel is NOT"),
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
    SaveLevel = Cast<USavedLevelData>(UGameplayStatics::LoadGameFromSlot(LevelDataSlot, 0));

    if (!SaveLevel)
    {
        SaveLevel = Cast<USavedLevelData>(UGameplayStatics::CreateSaveGameObject(
            USavedLevelData::StaticClass()));

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
