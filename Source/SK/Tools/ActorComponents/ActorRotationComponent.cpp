//

// Base:
#include "ActorRotationComponent.h"

// UE:
#include "Kismet/KismetMathLibrary.h"

// Interaction:
#include "SK/ChessMans/ChessMan.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values for this component's properties
UActorRotationComponent::UActorRotationComponent()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryComponentTick.bCanEverTick = true; // Warning: Принудительно
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void UActorRotationComponent::BeginPlay()
{
    Super::BeginPlay();

}

void UActorRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    RotationForTick(DeltaTime);
}

void UActorRotationComponent::OnComponentCreated()
{
    Super::OnComponentCreated();

    CurrentComponentInit();
}

void UActorRotationComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
    Super::OnComponentDestroyed(bDestroyingHierarchy);
}
//--------------------------------------------------------------------------------------



/* ---   This Component   --- */

void UActorRotationComponent::CurrentComponentInit()
{
    CurrentActor = Cast<AChessMan>(GetOwner());

    if (!CurrentActor)
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': CurrentActor is NOT"),
            *GetNameSafe(this));
    }
}
//--------------------------------------------------------------------------------------



/* ---   ChessMan Rotate   --- */

void UActorRotationComponent::RotateToLocation(const FVector& iPoint)
{
    if (CurrentActor)
    {
        StartRotation = CurrentActor->GetActorRotation().Yaw;
        EndRotation = UKismetMathLibrary::FindLookAtRotation(CurrentActor->GetActorLocation(), iPoint).Yaw;
        bIsRotatedToNewRotation = true;
    }
}

void UActorRotationComponent::RotationForTick(const float& DeltaTime)
{
    // Контроль перемещения
    if (bIsRotatedToNewRotation)
    {
        FRotator lNewRotation = CurrentActor->GetActorRotation();
        float lCurrentRotation = lNewRotation.Yaw;

        // Контроль близости к новой ротации
        if (abs(lCurrentRotation - EndRotation) < MinStep)
        {
            lNewRotation.Yaw = EndRotation;

            CurrentActor->SetActorRotation(lNewRotation);
            bIsRotatedToNewRotation = false;

            OnCompletedRotate.Broadcast();
        }
        else
        {
            // Рассчитанная скорость Yaw
            float lNewSpeed_Yaw = (EndRotation - lCurrentRotation);

            // Корректировка направления
            if (int(lNewSpeed_Yaw / 180))
            {
                lNewSpeed_Yaw -= 360 * (lNewSpeed_Yaw > 0 ? 1 : -1);
            }


            /* ---   Приближение к Цели   --- */

            if (abs(lNewSpeed_Yaw * DecelerationCoefficient) > MaxSpeed)
            {
                // Ограничение скорости с сохранением знака
                lNewSpeed_Yaw = MaxSpeed * (lNewSpeed_Yaw > 0 ? 1 : -1);
            }
            else
            {
                lNewSpeed_Yaw *= DecelerationCoefficient;
            }
            //-------------------------------------------


            /* ---   Отдаление от предыдущей Цели   --- */

            if (bControlSpeedAtStart)
            {
                // Пройденный поворот
                float lRotateTraveled = lCurrentRotation - StartRotation;

                // Корректировка направления
                if (int(lRotateTraveled / 180))
                {
                    lRotateTraveled -= 360 * (lRotateTraveled > 0 ? 1 : -1);
                }

                if (abs(lNewSpeed_Yaw) > abs(lRotateTraveled * AccelerationCoefficient))
                {
                    lNewSpeed_Yaw = (abs(lRotateTraveled * AccelerationCoefficient) + MinStep) * (lNewSpeed_Yaw > 0 ? 1 : -1);
                }
            }
            //-------------------------------------------

            //UE_LOG(LogTemp, Error, TEXT("'%s': lCurrentRotation = %f"),
            //    *GetNameSafe(this), lCurrentRotation);

            //UE_LOG(LogTemp, Warning, TEXT("'%s': lNewSpeed_Yaw = %f"),
            //    *GetNameSafe(this), lNewSpeed_Yaw);

            lNewRotation.Yaw = lNewSpeed_Yaw * DeltaTime;
            CurrentActor->AddActorWorldRotation(lNewRotation);
        }
    }
}
//-------------------------------------------
