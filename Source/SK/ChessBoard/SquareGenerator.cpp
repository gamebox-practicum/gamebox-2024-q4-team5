// 

// Base:
#include "SquareGenerator.h"

// Interaction:
#include "Square.h"
#include "SquareStruct.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values
ASquareGenerator::ASquareGenerator()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Preliminarily

}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

// Called when the game starts or when spawned
void ASquareGenerator::BeginPlay()
{
    Super::BeginPlay();

}

void ASquareGenerator::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    Preview();
}
//--------------------------------------------------------------------------------------



/* ---   Preview   --- */

void ASquareGenerator::Preview()
{
    DeleteAllSquares();
    CreateGeneratedSquares();
}

void ASquareGenerator::DeleteAllSquares()
{
    for (auto Data : AllSquare)
    {
        Data->Destroy();
    }
    AllSquare.Empty();
    BlockSize = FVector::ZeroVector;
}
//--------------------------------------------------------------------------------------



/* ---   Generator   --- */

void ASquareGenerator::CreateGeneratedSquares()
{
    for (int32 x = 0; x < NumberAlongAxes.X; ++x)
    {
        for (int32 y = 0; y < NumberAlongAxes.Y; ++y)
        {
            CreateSquare(FIndex2D(x, y));
        }
    }
}

void ASquareGenerator::CreateSquare(const FIndex2D& iXY)
{
    ASquare* lSquare = nullptr;

    if (BlockSize.IsZero())
    {
        lSquare = GetWorld()->SpawnActor<ASquare>(BlockType.Get(), FTransform());

        GetSquareSize(lSquare);

        lSquare->SetActorLocation(GetLocationForSquare(iXY));
    }
    else
    {
        lSquare = GetWorld()->SpawnActor<ASquare>(BlockType.Get(), GetLocationForSquare(iXY), FRotator());
    }

    SetSquareData(lSquare, SquareDataGeneration(iXY));

    AllSquare.Add(lSquare);
}

void ASquareGenerator::GetSquareSize(const ASquare* iBlock)
{
    if (iBlock)
    {
        BlockSize = iBlock->BlockMesh->Bounds.BoxExtent * 2;

        PointOffset = FVector(
            (BlockSize.X + Gap.X) * (NumberAlongAxes.X - 1) / 2, // Середина по оси X
            (BlockSize.Y + Gap.Y) * (NumberAlongAxes.Y - 1) / 2, // Середина по оси Y
            BlockSize.Z); // Высота по оси Z
        // PS: Смещение указано относительно Генератора
    }
}

FVector ASquareGenerator::GetLocationForSquare(const FIndex2D& iXY) const
{
    if (NumberAlongAxes.Within(iXY))
    {
        return (BlockSize + Gap) * iXY - PointOffset + GetActorLocation();
    }

    return FVector::ZeroVector;
}
//--------------------------------------------------------------------------------------



/* ---   Generator : SquareData   --- */

void ASquareGenerator::SetSquareData(ASquare* iBlock, FSquareData iData)
{
    if (iBlock)
    {
        iBlock->SetData(iData);
        // Предварительно
    }
}

FSquareData ASquareGenerator::SquareDataGeneration(const FIndex2D& iXY)
{
    FSquareData rData;

    rData.TypeBlockMaterial = GetMaterialNumber(iXY);

    return rData;
}

int32 ASquareGenerator::GetMaterialNumber(const FIndex2D& iXY)
{
    return (iXY.X + iXY.Y) % 2;
}
//--------------------------------------------------------------------------------------
