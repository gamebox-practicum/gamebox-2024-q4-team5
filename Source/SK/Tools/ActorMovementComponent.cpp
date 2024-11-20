//

// Base:
#include "ActorMovementComponent.h"

// UE:
#include "GameFramework/Actor.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values for this component's properties
UActorMovementComponent::UActorMovementComponent()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryComponentTick.bCanEverTick = true; // Warning: Принудительно

}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void UActorMovementComponent::BeginPlay()
{
    Super::BeginPlay();

}

void UActorMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    MovementForTick(DeltaTime);
}

void UActorMovementComponent::OnComponentCreated()
{
    Super::OnComponentCreated();

    CurrentComponentInit();
}

void UActorMovementComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
    Super::OnComponentDestroyed(bDestroyingHierarchy);
}
//--------------------------------------------------------------------------------------



/* ---   This Component   --- */

void UActorMovementComponent::CurrentComponentInit()
{
    CurrentActor = Cast<AActor>(GetOwner());

    if (!CurrentActor)
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': CurrentActor is NOT"),
            *GetNameSafe(this));
    }
}
//--------------------------------------------------------------------------------------



/* ---   Actor Movement   --- */

void UActorMovementComponent::MoveToLocation(const FVector& iPoint)
{
    if (!bIsMovingToNewLocation && CurrentActor)
    {
        if (bControlSpeedAtStart)
        {
            StartLocation = CurrentActor->GetActorLocation();
        }

        EndLocation = iPoint;
        bIsMovingToNewLocation = true;
    }
}

void UActorMovementComponent::MovementForTick(const float& DeltaTime)
{
    // Контроль перемещения
    if (bIsMovingToNewLocation)
    {
        FVector lCurrentLocation = CurrentActor->GetActorLocation();

        // Контроль близости к новой локации
        if ((lCurrentLocation - EndLocation).Size() < MinStep)
        {
            CurrentActor->SetActorLocation(EndLocation);
            bIsMovingToNewLocation = false;

            OnCompletedMove.ExecuteIfBound();
        }
        else
        {
            // Шаг в векторном исполнении
            FVector lNewSpeed_Vector = (EndLocation - lCurrentLocation);

            if (lNewSpeed_Vector.Size() * DecelerationCoefficient > MaxSpeed)
            {
                lNewSpeed_Vector = lNewSpeed_Vector.GetSafeNormal() * MaxSpeed;
            }
            else
            {
                lNewSpeed_Vector *= DecelerationCoefficient;
            }

            if (bControlSpeedAtStart)
            {
                float lDistanceTraveled = (lCurrentLocation - StartLocation).Size();

                if (lNewSpeed_Vector.Size() > lDistanceTraveled * AccelerationCoefficient)
                {
                    lNewSpeed_Vector = lNewSpeed_Vector.GetSafeNormal() * (lDistanceTraveled * AccelerationCoefficient + MinStep);
                }
            }

            //UE_LOG(LogTemp, Error, TEXT("'%s': lNewSpeed_Vector = %s"),
            //    *GetNameSafe(this), *(lNewSpeed_Vector.ToString()));

            // Сделать шаг (Плавное перемещение)
            CurrentActor->AddActorLocalOffset(lNewSpeed_Vector * DeltaTime);
        }
    }
}
//--------------------------------------------------------------------------------------
