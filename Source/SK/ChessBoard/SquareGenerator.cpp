// 

// Base:
#include "SquareGenerator.h"

// UE:
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"

// Interaction:
#include "Square.h"
#include "SquareComponent.h"
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
    if (SquareType)
    {
        if (NumberAlongAxes != TDArraySquares.Num())
        {
            DeleteAllSquares();
            CreateGeneratedSquares();
        }
        else
        {
            DeleteAllSquareComponents();
        }

        CreateGeneratedSquareComponents();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': SquareType is NOT"),
            *GetNameSafe(this));
    }
}

void ASquareGenerator::DeleteAllSquares()
{
    for (auto& lSquare : GetAllActors<ASquare>(VerificationTag))
    {
        lSquare->Destroy();
    }
    BlockSize = FVector::ZeroVector;
}

void ASquareGenerator::DeleteAllSquareComponents()
{
    UActorComponent* lComponent = nullptr;

    for (auto& lSquare : GetAllActors<ASquare>(VerificationTag))
    {
        lComponent = lSquare->GetComponentByClass(USquareComponent::StaticClass());

        if (lComponent)
            lComponent->DestroyComponent();
    }
}
//--------------------------------------------------------------------------------------



/* ---   Generator   --- */
// Warning: Следует переделать как шаблонные функции универсального Генератора

void ASquareGenerator::CreateGeneratedSquares()
{
    // Сброс массива
    TDArraySquares.Empty();

    // Создание элементов массива X
    TDArraySquares.SetNum(NumberAlongAxes);

    for (int32 x = 0; x < NumberAlongAxes.X; ++x)
    {
        for (int32 y = 0; y < NumberAlongAxes.Y; ++y)
        {
            // Создание Клетки и добавление её в массив по соответствующему индексу
            TDArraySquares.SetByIndex(CreateSquare(FIndex2D(x, y)), x, y);
        }
    }
}

ASquare* ASquareGenerator::CreateSquare(const FIndex2D& iXY)
{
    ASquare* lSquare = nullptr;

    if (BlockSize.IsZero())
    {
        lSquare = GetWorld()->SpawnActor<ASquare>(SquareType.Get(), FTransform());

        GetSquareSize(lSquare);

        lSquare->SetActorLocation(GetLocationForSquare(iXY));
    }
    else
    {
        lSquare = GetWorld()->SpawnActor<ASquare>(SquareType.Get(), GetLocationForSquare(iXY), FRotator::ZeroRotator);
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
    return &TDArraySquares;
}
//--------------------------------------------------------------------------------------



/* ---   Square Components   --- */

void ASquareGenerator::CreateGeneratedSquareComponents()
{
    if (SquareComponentTable)
    {
        // Массив данных, получаемых из DataTable
        TArray<FSquareComponentData*> lSquareComponentData;

        // Контекст для определения в случае ошибки (см. UDataTable::GetAllRows)
        FString lContext = "CreateGeneratedSquareComponents";

        // Получить массив данных из DataTable
        SquareComponentTable->GetAllRows<FSquareComponentData>(lContext, lSquareComponentData);

        // Указатель на создаваемый компонент
        USquareComponent* lNewComponent = nullptr;

        // Указатель на выбранную клетку
        ASquare* lCurrentSquare = nullptr;

        // Создать компонент в указанной клетке
        for (auto& lData : lSquareComponentData)
        {
            lCurrentSquare = TDArraySquares.GetByIndex(lData->Position);

            // Создаём компонент и получаем на него указатель
            lNewComponent = Cast<USquareComponent>(
                lCurrentSquare->AddComponentByClass(
                    lData->Type,
                    false,
                    FTransform(),
                    false));

            // "Верификация" и сохранение компонента
            if (lNewComponent)
            {
                lCurrentSquare->AddInstanceComponent(lNewComponent);
                lNewComponent->RegisterComponent();
                lNewComponent->ComponentTags.Add(VerificationTag);
            }
        }
    }
}
//--------------------------------------------------------------------------------------
