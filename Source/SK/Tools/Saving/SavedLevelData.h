// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

// Interaction:
#include "LevelData.h"

// Generated:
#include "SavedLevelData.generated.h"
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API USavedLevelData : public USaveGame
{
    GENERATED_BODY()

public:

    // Данные уровня для последующего сохранения
    UPROPERTY(BlueprintReadOnly)
    FLevelData LevelData;
};
