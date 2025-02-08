// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

// Interaction:
#include "LevelData.h"
#include "LevelSelectionData.h"

// Generated:
#include "SaveLevel.generated.h"
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API USaveLevel : public USaveGame
{
    GENERATED_BODY()

public:

    // Данные о выборе уровня
    UPROPERTY(BlueprintReadOnly)
    FLevelSelectionData LevelSelectionData;

    // Данные уровня для последующего сохранения
    UPROPERTY(BlueprintReadOnly)
    FLevelData LevelData;
};
//--------------------------------------------------------------------------------------
