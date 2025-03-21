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

protected:

    /* ---   Base   --- */

    virtual void BeginPlay() override;
    //-------------------------------------------



    /* ---   Events   --- */

    /** Событие Выигрышной игры */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "Game Status",
        meta = (DisplayName = "Winning Game"))
    void EventWinningGame();

    /** Событие Проигрышной игры */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "Game Status",
        meta = (DisplayName = "Losing Game"))
    void EventLosingGame();
    //-------------------------------------------



public:

    /* ---   Events   --- */

    /** Установить Игру как Выигрышную */
    void SetWinningGame();

    /** Установить Игру как Проигрышную */
    void SetLosingGame();

    /** Получить ссылку на флаг завершения игры */
    const bool* GetFlagGameOver() const;
    //-------------------------------------------



    /* ---   Focus   --- */

    UFUNCTION(BlueprintImplementableEvent)
    void OnWindowsLostFocus();

    UFUNCTION(BlueprintImplementableEvent)
    void OnWindowsGainFocus();
    //-------------------------------------------



    /* ---   Note System   --- */

    //-------------------------------------------



private:

    /* ---   Events   --- */

    // Флаг завершения игры (необходим для выхода из цикла лишних действий)
    bool bIsGameOver = false;
    //-------------------------------------------



    /* ---   Focus   --- */

    void OnWindowFocusChanged(bool bIsFocused);
    //-------------------------------------------
};
