//

// Base:
#include "Square.h"

// UE:
#include "Kismet/GameplayStatics.h"

// Interaction:
#include "SK/Core/SK_Character.h"
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

    // Главный Меш
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

void ASquare::NotifyActorOnClicked(FKey ButtonReleased)
{
    Super::NotifyActorOnClicked(ButtonReleased);

    // Предварительное присвоение
    ASK_Character* lCharacter = Cast<ASK_Character>(GetWorld()->GetFirstPlayerController()->GetPawn());
    // Warning: Потребуется переделать, если переделывать на Multiplayer

    if (lCharacter)
    {
        lCharacter->MoveToSquare(this);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': lCharacter is NOT"), *GetNameSafe(this));
    }
}
//--------------------------------------------------------------------------------------



/* ---   Generator   --- */

void ASquare::UpdateMaterialByType(int32& iNumType)
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
                UE_LOG(LogTemp, Error, TEXT("%s: UpdateMaterialByType: Materials is NOT"), *GetNameSafe(this));
        }
    }
}
//--------------------------------------------------------------------------------------



/* ---   Data   --- */

void ASquare::SetData(FSquareData iData)
{


    UpdateMaterialByType(iData.TypeBlockMaterial);
}

const FSquareData& ASquare::GetData()
{
    return SquareData;
}

void ASquare::OccupySquare(const EWarringPartiesType& iWarringPartiesType)
{
    SquareData.WarringPartiesType = iWarringPartiesType;
}
//--------------------------------------------------------------------------------------
