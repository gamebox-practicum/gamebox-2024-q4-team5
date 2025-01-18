// 

// Base:
#include "TimeBeaconGenerator.h"

// UE:
#include "Kismet/GameplayStatics.h"

// Interaction:
#include "TimeBeacon.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values
ATimeBeaconGenerator::ATimeBeaconGenerator()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Preliminarily
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

// Called when the game starts or when spawned
void ATimeBeaconGenerator::BeginPlay()
{
    Super::BeginPlay();

}
//--------------------------------------------------------------------------------------



/* ---   Re Generate   --- */

void ATimeBeaconGenerator::ReGenerate()
{
    DeleteAllTimeBeacons();
    CreateGeneratedTimeBeacons();
}

void ATimeBeaconGenerator::DeleteAllTimeBeacons()
{
    TArray<AActor*> lResultActors;

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATimeBeacon::StaticClass(), VerificationTag, lResultActors);

    for (auto& lBeacon : lResultActors)
    {
        lBeacon->Destroy();
    }
    AllTimeBeacon.Empty();
}

void ATimeBeaconGenerator::CreateGeneratedTimeBeacons()
{
    for (int32 i = 0; i < NumberOfSquaresAlongAxes.X * 2; ++i)
    {
        AllTimeBeacon.Add(CreateTimeBeacon(i));
    }
}

ATimeBeacon* ATimeBeaconGenerator::CreateTimeBeacon(const int32& iX)
{
    if (BeaconType)
    {
        // Создание Маяка по рассчитанным параметрами
        ATimeBeacon* lBeacon = GetWorld()->SpawnActor<ATimeBeacon>(
            BeaconType.Get(),
            GetLocationForTimeBeacon(iX),
            iX % 2 ? FRotator(0.f, -180.f, 0.f) : FRotator(0.f, 0.f, 0.f));;

        if (lBeacon)
        {
            // Тег-маркировка Маяка.
            // Необходим для удаления только Генерируемых Триггеров
            lBeacon->Tags.Add(VerificationTag);

            return lBeacon;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': BeaconType is NOT"),
            *GetNameSafe(this));
    }

    return nullptr;
}
//--------------------------------------------------------------------------------------



/* ---   Generator   --- */

void ATimeBeaconGenerator::SetTimeForBeacons(const float& iTime)
{
    StopTimeBeaconGenerator();

    PartOfTime = iTime / NumberOfSquaresAlongAxes.X;

    for (auto& lBeacon : AllTimeBeacon)
    {
        lBeacon->SetTimeForTimer(PartOfTime);
    }
}

void ATimeBeaconGenerator::StopTimeBeaconGenerator()
{
    CounterForTimer = NumberOfSquaresAlongAxes.X;

    for (auto& lBeacon : AllTimeBeacon)
    {
        lBeacon->StopTimer();
    }

    GetWorldTimerManager().ClearTimer(Timer_Counter);
}

void ATimeBeaconGenerator::PlayTimeBeaconGenerator()
{
    // Запуск таймера: Поочерёдный триггер для Маяков
    GetWorldTimerManager().SetTimer(
        Timer_Counter,
        this,
        &ATimeBeaconGenerator::TimerAction_TriggerForBeacon,
        PartOfTime,
        true,
        0.0f); // Запустить реакцию таймера сразу
}

void ATimeBeaconGenerator::UpdateStage(const int32& CurrentStageNum)
{
    for (auto& Beacon : AllTimeBeacon)
    {
        Beacon->EventOnNextStage(CurrentStageNum);
    }
}

FVector ATimeBeaconGenerator::GetLocationForTimeBeacon(const int32& iInd)
{
    // Очерёдность индекса: Слева направо, снизу вверх
    return FVector(
        BlockSize.X * (iInd / 2),
        (BlockSize.Y * (NumberOfSquaresAlongAxes.Y + 1) / 2) * (iInd % 2 ? 1 : -1), // Определение по оси Y с учётом стороны
        0)
        + GetActorLocation(); // Добавление текущей позиции Генератора
}

void ATimeBeaconGenerator::TimerAction_TriggerForBeacon()
{
    --CounterForTimer;

    if (CounterForTimer >= 0)
    {
        AllTimeBeacon[CounterForTimer * 2]->PlayTimer();
        AllTimeBeacon[CounterForTimer * 2 + 1]->PlayTimer();
    }

    if (CounterForTimer == 0)
    {
        GetWorldTimerManager().ClearTimer(Timer_Counter);
    }
}
//--------------------------------------------------------------------------------------
