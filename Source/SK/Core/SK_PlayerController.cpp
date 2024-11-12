// 

// Base:
#include "SK_PlayerController.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

ASK_PlayerController::ASK_PlayerController()
{
    // Установка вызова функции Tick() в каждом кадре.
    PrimaryActorTick.bCanEverTick = true;
    //-------------------------------------------


    /* ---   Mouse   --- */

    // Дистанция подбора предмета (трассировки наведения Мыши)
    HitResultTraceDistance = 200.f;

    // Включение реакций от наведения мыши данным контроллером
    bEnableMouseOverEvents = true;

    // Включение реакций от нажатия мыши данным контроллером
    bEnableClickEvents = true;
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void ASK_PlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    SetMouseToCenter();
}
//--------------------------------------------------------------------------------------



/* ---   Mouse   --- */

void ASK_PlayerController::SetMouseToCenter()
{
    if (GetPawn())
    {
        if (!IsPaused())
        {
            if (GetMousePosition(MousePositionX, MousePositionY))
            {
                GetViewportSize(SizeCenterX, SizeCenterY);

                SizeCenterX /= 2;
                SizeCenterY /= 2;

                if (MousePositionX != SizeCenterX || MousePositionY != SizeCenterY)
                {
                    SetMouseLocation(SizeCenterX, SizeCenterY);
                }
            }
        }
    }
}
//--------------------------------------------------------------------------------------
