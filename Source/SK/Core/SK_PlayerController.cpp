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

    EnableMouseEvents(true);

    ClickEventKeys.Add(EKeys::RightMouseButton);
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
            && !*bpIsGameOver)
        {
            if (!IsPaused())
            {
                if (GetMousePosition(MousePositionX, MousePositionY))
                {
                    GetViewportSize(SizeCenterX, SizeCenterY);

                    if (MousePositionX != int32(SizeCenterX / 2) || MousePositionY != int32(SizeCenterY / 2))
                    {
                        SetMouseLocation(SizeCenterX / 2, SizeCenterY / 2);
                    }
                }
            }
        }
        else
        {
            EnableMouseEvents(false);

            // Флаг: Прекратить контроль Мыши
            bMouseToCenter = false;
        }
    }
}

void ASK_PlayerController::EnableMouseEvents(const bool& bControl)
{
    // Выключение реакций от наведения мыши данным контроллером
    bEnableMouseOverEvents = bControl;

    // Выключение реакций от нажатия мыши данным контроллером
    bEnableClickEvents = bControl;
}
//--------------------------------------------------------------------------------------
