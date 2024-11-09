// 

// Base:
#include "SquareGenerator.h"

// UE:
#include "Kismet/GameplayStatics.h"

// Interaction:
#include "Square.h"
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

    //ReGenerate();
}
//--------------------------------------------------------------------------------------



/* ---   Re Generate   --- */
// Warning: Следует переделать как шаблонные функции универсального Генератора

void ASquareGenerator::ReGenerate()
{
    DeleteAllSquares();
    CreateGeneratedSquares();
}

void ASquareGenerator::DeleteAllSquares()
{
    for (auto& lSquare : GetAllActors<ASquare>(VerificationTag))
    {
        lSquare->Destroy();
    }
    BlockSize = FVector::ZeroVector;
}
//--------------------------------------------------------------------------------------



/* ---   Generator   --- */
// Warning: Следует переделать как шаблонные функции универсального Генератора

void ASquareGenerator::CreateGeneratedSquares()
{
    // Сброс массива
    TDArraySquares_Test.Empty();

    // Создание элементов массива X
    TDArraySquares_Test.SetNum(NumberAlongAxes);

    for (int32 x = 0; x < NumberAlongAxes.X; ++x)
    {
        for (int32 y = 0; y < NumberAlongAxes.Y; ++y)
        {
            // Создание Клетки и добавление её в массив по соответствующему индексу
            TDArraySquares_Test.SetByIndex(CreateSquare(FIndex2D(x, y)), x, y);
        }
    }
}

ASquare* ASquareGenerator::CreateSquare(const FIndex2D& iXY)
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
        lSquare = GetWorld()->SpawnActor<ASquare>(BlockType.Get(), GetLocationForSquare(iXY), FRotator::ZeroRotator);
    }

    // Тег-маркировка Клетки.
    // Необходим для удаления только Генерируемых Клеток
    lSquare->Tags.Add(VerificationTag);

    SetSquareData(lSquare, SquareDataGeneration(iXY));

    return lSquare;
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



/* ---   Generator | SquareData   --- */

void ASquareGenerator::SetSquareData(ASquare* iBlock, FSquareData iData)
{
    if (iBlock)
    {
        iBlock->SetData(iData);
    }
}

FSquareData ASquareGenerator::SquareDataGeneration(const FIndex2D& iXY)
{
    FSquareData rData;

    rData.PositionNumber = iXY;
    rData.TypeBlockMaterial = GetMaterialNumber(iXY);

    return rData;
}

int32 ASquareGenerator::GetMaterialNumber(const FIndex2D& iXY)
{
    return (iXY.X + iXY.Y) % 2;
}
//--------------------------------------------------------------------------------------



/* ---   Get Data   --- */

FSquareArray2D* ASquareGenerator::GetPointerToAllSquares()
{
    return &TDArraySquares_Test;
}
//--------------------------------------------------------------------------------------
