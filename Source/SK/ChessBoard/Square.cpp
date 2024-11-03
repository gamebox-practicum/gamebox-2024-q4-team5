//

// Base:
#include "Square.h"

#include "SquareStruct.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values
ASquare::ASquare()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Preliminarily



    /* ---   Components   --- */

    // Корневой компонент
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Главный Мешь
    BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Block Mesh"));
    BlockMesh->SetupAttachment(RootComponent);
    BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, -25.f));
    BlockMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.5f));
    BlockMesh->SetCastShadow(false);
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

// Called when the game starts or when spawned
void ASquare::BeginPlay()
{
    Super::BeginPlay();

}
//--------------------------------------------------------------------------------------



/* ---   Interaction   --- */

void ASquare::NotifyActorBeginCursorOver()
{
    Super::NotifyActorBeginCursorOver();

    BlockMesh->SetRenderCustomDepth(true);
}

void ASquare::NotifyActorEndCursorOver()
{
    Super::NotifyActorEndCursorOver();

    BlockMesh->SetRenderCustomDepth(false);
}
//--------------------------------------------------------------------------------------



/* ---   Generator   --- */

void ASquare::SetData(FSquareData iData)
{
    SetMaterialByType(iData.TypeBlockMaterial);
}

void ASquare::SetMaterialByType(int32& iNumType)
{
    if (BlockMesh)
    {
        if (iNumType && BlockMaterialsB)
        {
            BlockMesh->SetMaterial(0, BlockMaterialsB);
        }
        else
        {
            if (BlockMaterialsA)
                BlockMesh->SetMaterial(0, BlockMaterialsA);
            else
                UE_LOG(LogTemp, Error, TEXT("%s: SetMaterialByType: Materials is NOT"), *GetNameSafe(this));
        }
    }
}
//--------------------------------------------------------------------------------------
