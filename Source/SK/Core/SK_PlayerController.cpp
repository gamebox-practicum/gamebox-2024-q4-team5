// 

// Base:
#include "SK_PlayerController.h"

// Interaction:
#include "SK_GameMode.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

ASK_PlayerController::ASK_PlayerController()
{
    // Установка вызова функции Tick() в каждом кадре.
    PrimaryActorTick.bCanEverTick = true; // Принудительно
    //-------------------------------------------


    /* ---   Mouse   --- */

    // Дистанция подбора предмета (трассировки наведения Мыши)
    HitResultTraceDistance = 280.f;

    // Включение реакций от наведения мыши данным контроллером
    bEnableMouseOverEvents = true;

    // Включение реакций от нажатия мыши данным контроллером
    bEnableClickEvents = true;
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------




/* ---   Base   --- */

void ASK_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    bpIsGameOver = Cast<ASK_GameMode>(GetWorld()->GetAuthGameMode())->GetFlagGameOver();
}

void ASK_PlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    SetMouseToCenter();
}
//--------------------------------------------------------------------------------------



/* ---   Mouse   --- */

void ASK_PlayerController::SetMouseToCenter()
{
    if (bMouseToCenter)
    {
        if (GetPawn()
            && !IsPaused()
            && !*bpIsGameOver)
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
        else
        {
            // Выключение реакций от наведения мыши данным контроллером
            bEnableMouseOverEvents = false;

            // Выключение реакций от нажатия мыши данным контроллером
            bEnableClickEvents = false;

            // Флаг: Прекратить контроль Мыши
            bMouseToCenter = false;
        }
    }
}
//--------------------------------------------------------------------------------------
