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



/* ---   Level Saving   --- */

void USK_GameInstance::SaveLevelData(const FLevelData& iLevelData) const
{
    if (SaveLevel)
    {
        SaveLevel->LevelData = iLevelData;

        UGameplayStatics::SaveGameToSlot(SaveLevel, LevelDataSlot, 0);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s'::SaveLevelData: SaveLevel is NOT"),
            *GetNameSafe(this));
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

void USK_GameInstance::ClearLevelData()
{
    SaveLevelData(FLevelData::Empty);
}
//--------------------------------------------------------------------------------------


