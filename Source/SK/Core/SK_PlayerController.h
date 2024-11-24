// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// Generated:
#include "SK_PlayerController.generated.h"
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API ASK_PlayerController : public APlayerController
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    ASK_PlayerController();
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Base   --- */

    /** Функция, вызываемая каждый кадр в этом Акторе */
    virtual void Tick(float DeltaSeconds) override;
    //-------------------------------------------



    /* ---   Mouse   --- */

    /** Включить события мыши */
    void EnableMouseEvents(const bool& bControl);
    //-------------------------------------------



private:

    /* ---   Mouse   --- */

    // Текущая позиция мыши по оси X
    float MousePositionX = 0;

    // Текущая позиция мыши по оси Y
    float MousePositionY = 0;

    // Текущий центр экрана по оси X
    int32 SizeCenterX = 0;

    // Текущий центр экрана по оси Y
    int32 SizeCenterY = 0;

    // Флаг завершения игры, полученный из GameMode
    const bool* bpIsGameOver;

    // Флаг контроля Мыши в центре Экрана
    bool bMouseToCenter = true;

    //

    /** Установить Мышь в центр экрана */
    void SetMouseToCenter();
    //-------------------------------------------
};
