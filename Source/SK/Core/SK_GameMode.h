// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

// Generated:
#include "SK_GameMode.generated.h"
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API ASK_GameMode : public AGameMode
{
    GENERATED_BODY()

public:

    /* ---   Events   --- */

    /** Событие Выигрышной игры */
    UFUNCTION(BlueprintImplementableEvent, Category = "Game Status",
        meta = (DisplayName = "Winning Game"))
    void EventWinningGame();

    /** Событие Проигрышной игры */
    UFUNCTION(BlueprintImplementableEvent, Category = "Game Status",
        meta = (DisplayName = "Losing Game"))
    void EventLosingGame();
    //-------------------------------------------
};
