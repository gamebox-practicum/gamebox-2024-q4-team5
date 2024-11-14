// 

// Base:
#include "TimeBeacon.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values
ATimeBeacon::ATimeBeacon()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Preliminarily



    /* ---   Components   --- */

    // Корневой компонент
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Меш Шахматной Фигуры со скелетом
    ColumnSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Column Skeletal Mesh"));
    ColumnSkeletalMesh->SetupAttachment(RootComponent);

    // Статичный Меш Шахматной Фигуры
    ColumnStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Column Static Mesh"));
    ColumnStaticMesh->SetupAttachment(RootComponent);
    ColumnStaticMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

// Called when the game starts or when spawned
void ATimeBeacon::BeginPlay()
{
    Super::BeginPlay();

}

void ATimeBeacon::Cleaning()
{
    if (ColumnSkeletalMesh && !(ColumnSkeletalMesh->SkeletalMesh))
    {
        ColumnSkeletalMesh->DestroyComponent();
    }

    if (ColumnStaticMesh && !(ColumnStaticMesh->GetStaticMesh()))
    {
        ColumnStaticMesh->DestroyComponent();
    }
}
//--------------------------------------------------------------------------------------



/* ---   Timer   --- */

void ATimeBeacon::SetTimeForTimer(const float& iTime)
{
    EventSetTimeForTimer(iTime);
}

void ATimeBeacon::PlayTimer()
{
    EventPlayTimer();
}

void ATimeBeacon::StopTimer()
{
    EventStopTimer();
}
//--------------------------------------------------------------------------------------
