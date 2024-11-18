// 

// Base:
#include "SK_GameMode.h"
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

const bool* ASK_GameMode::GetFlagGameOver()
{
    return &bIsGameOver;
}
//--------------------------------------------------------------------------------------
