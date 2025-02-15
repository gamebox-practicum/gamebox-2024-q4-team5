//

// Base:
#include "SK_GameMode.h"
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void ASK_GameMode::BeginPlay()
{
    Super::BeginPlay();

    FSlateApplication::Get().OnApplicationActivationStateChanged().AddUObject(this, &ASK_GameMode::OnWindowFocusChanged);
}
//--------------------------------------------------------------------------------------



/* ---   Events   --- */

void ASK_GameMode::SetWinningGame()
{
    bIsGameOver = true;

    EventWinningGame();
}

void ASK_GameMode::SetLosingGame()
{
    bIsGameOver = true;

    EventLosingGame();
}

const bool* ASK_GameMode::GetFlagGameOver() const
{
    return &bIsGameOver;
}
//--------------------------------------------------------------------------------------



/* ---   Focus   --- */

void ASK_GameMode::OnWindowFocusChanged(const bool bIsFocused)
{
    if (bIsFocused)
    {
        OnWindowsGainFocus();
    }
    else
    {
        OnWindowsLostFocus();
    }
}
//--------------------------------------------------------------------------------------
