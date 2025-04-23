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
