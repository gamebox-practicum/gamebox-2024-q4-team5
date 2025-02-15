// 

// Base:
#include "SquareGenerator.h"

// UE:
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

// Interaction:
#include "Square.h"
#include "SquareComponent.h"
#include "SK/ChessOperators/ChessOperator.h"
#include "SK/ChessOperators/StageTrigger.h"
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
    if (SquareType
        && NumberAlongAxes.X > 0
        && NumberAlongAxes.Y > 0)
    {
        if (NumberAlongAxes != TDArraySquares.Num())
        {
            RecreateBoard();
        }
        else
        {
            DeleteAllSquareComponents();
        }

        CreateGeneratedSquareComponents(SquareComponentTable);
    }
    else if (!SquareType)
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': SquareType is NOT"),
            *GetNameSafe(this));
    }
    else if (NumberAlongAxes.X <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': NumberOfSquaresAlongAxes.X <= 0"),
            *GetNameSafe(this));
    }
    else if (NumberAlongAxes.Y <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': NumberOfSquaresAlongAxes.Y <= 0"),
            *GetNameSafe(this));
    }
}

void ASquareGenerator::DeleteAllSquares()
{
    TArray<AActor*> lResultActors;

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASquare::StaticClass(), VerificationTag, lResultActors);

    for (auto& lSquare : lResultActors)
    {
        lSquare->Destroy();
    }
    BlockSize = FVector::ZeroVector;
}

void ASquareGenerator::DeleteAllSquareComponents()
{
    TArray<AActor*> lResultActors;
    TArray<USquareComponent*> AllSquareComponents;

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASquare::StaticClass(), VerificationTag, lResultActors);

    for (auto& lSquare : lResultActors)
    {
        lSquare->GetComponents<USquareComponent>(AllSquareComponents);

        Cast<ASquare>(lSquare)->OccupySquare(EWarringPartiesType::NONE);

        for (auto& Component : AllSquareComponents)
        {
            Component->DestroyComponent();
        }
    }
}

void ASquareGenerator::DeleteStageTriggers()
{
    TArray<AActor*> lResultActors;

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AStageTrigger::StaticClass(), VerificationTag, lResultActors);

    for (auto& lTrigger : lResultActors)
    {
        lTrigger->Destroy();
    }
}

void ASquareGenerator::RecreateBoard()
{
    DeleteAllSquares();
    CreateGeneratedSquares();

    DeleteStageTriggers();
    CreatStageTrigger();

    if (CurrentOperator)
    {
        CurrentOperator->UpdateCurrentTimeBeaconGenerator(BlockSize);
    }
}
//--------------------------------------------------------------------------------------



/* ---   Generator   --- */
// Warning: Следует переделать как шаблонные функции универсального Генератора

void ASquareGenerator::CreateGeneratedSquares()
{
    // Сброс массивов
    TDArraySquares.Empty();
    CorpsesPositionIndex.Empty();

    // Создание элементов массива
    TDArraySquares.SetNum(NumberAlongAxes);

    for (int32 x = 0; x < NumberAlongAxes.X; ++x)
    {
        for (int32 y = 0; y < NumberAlongAxes.Y; ++y)
        {
            // Создание Клетки и добавление её в массив по соответствующему индексу
            TDArraySquares.SetByIndex(CreateSquare(FIndex2D(x, y)), x, y);
        }
    }

    // Создание "буферного" (не игрового для AI) последнего ряда
    for (int32 y = 0; y < NumberAlongAxes.Y; ++y)
    {
        CreateSquare(FIndex2D(NumberAlongAxes.X, y));
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

    SetSquareData(lSquare, SquareDataGeneration(iXY));

    return lSquare;
}

void ASquareGenerator::GetSquareSize(const ASquare* iBlock)
{
    if (iBlock)
    {
        BlockSize = iBlock->BlockMesh->Bounds.BoxExtent * 2;

        PointOffset = FVector(
            0, // Без смещения по оси X
            BlockSize.Y * (NumberAlongAxes.Y - 1) / 2, // Середина по оси Y
            BlockSize.Z); // Высота по оси Z
        // PS: Смещение указано относительно Генератора
    }
}

FVector ASquareGenerator::GetLocationForSquare(const FIndex2D& iXY) const
{
    return BlockSize * iXY - PointOffset + GetActorLocation();
}

void ASquareGenerator::CreatStageTrigger()
{
    if (StageTriggerType)
    {
        // Создание Триггера по рассчитанным параметрами
        AStageTrigger* lTrigger = GetWorld()->SpawnActor<AStageTrigger>(
            StageTriggerType.Get(), GetLocationForStageTrigger(), FRotator::ZeroRotator);;

        if (lTrigger)
        {
            // Передача указателя на Оператор
            lTrigger->SetPointerToOperator(CurrentOperator);

            // Передача рассчитанного размера
            lTrigger->SetActorScale3D(GetScaleForStageTrigger());

            // Тег-маркировка Триггера.
            // Необходим для удаления только Генерируемых Триггеров
            lTrigger->Tags.Add(VerificationTag);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': StageTriggerType is NOT"),
            *GetNameSafe(this));
    }
}

FVector ASquareGenerator::GetLocationForStageTrigger()
{
    // Получение позиции крайней Клетки вдоль оси X
    float lXPosition = 0;

    // Проверка последнего этапа
    if (CurrentOperator->CurrentStageNum == CurrentOperator->TotalStageNum - 1)
    {
        lXPosition = GetLocationForSquare(NumberAlongAxes).X;
    }
    else
    {
        lXPosition = GetLocationForSquare(NumberAlongAxes - 1).X;
    }

    // Возврат локации: Центр последнего ряда
    return FVector(
        lXPosition, // Позиция крайней Клетки вдоль оси X
        GetActorLocation().Y,
        GetActorLocation().Z);
}

FVector ASquareGenerator::GetScaleForStageTrigger()
{
    // Возврат размера: Весь ряд
    return FVector(
        0.5f,
        NumberAlongAxes.Y - 1,
        1.0);
}
//--------------------------------------------------------------------------------------



/* ---   Generator | SquareData   --- */

void ASquareGenerator::SetSquareData(ASquare* iBlock, FSquareData iData)
{
    if (iBlock)
    {
        // Тег-маркировка Клетки.
        // Необходим для удаления только Генерируемых Клеток
        iBlock->Tags.Add(VerificationTag);

        iBlock->SetPointerToSquareGenerator(this);

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

bool ASquareGenerator::GetMaterialNumber(const FIndex2D& iXY)
{
    return bool((iXY.X + iXY.Y) % 2) != bWhiteSquareFirst;
}
//--------------------------------------------------------------------------------------



/* ---   Get Data   --- */

FSquareArray2D* ASquareGenerator::GetPointerToAllSquares()
{
    return &TDArraySquares;
}

void ASquareGenerator::SetPointerToOperator(AChessOperator* iCurrentOperator)
{
    CurrentOperator = iCurrentOperator;
}
//--------------------------------------------------------------------------------------



/* ---   Square Components   --- */

void ASquareGenerator::CreateGeneratedSquareComponents(const TArray<FSquareComponentData*>& iSquareComponentsData)
{
    // Указатель на создаваемый компонент
    USquareComponent* lNewComponent = nullptr;

    // Указатель на выбранную клетку
    ASquare* lCurrentSquare = nullptr;

    // Создать компонент в указанной клетке
    for (auto& lData : iSquareComponentsData)
    {
        lCurrentSquare = TDArraySquares.GetByIndex(lData->Position);

        if (lCurrentSquare)
        {
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
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("'%s': TDArraySquares [%d, %d] is NOT"),
                *GetNameSafe(this), lData->Position.X, lData->Position.Y);
        }
    }
}

void ASquareGenerator::CreateGeneratedSquareComponents(UDataTable* iSquareComponentTable)
{
    if (iSquareComponentTable)
    {
        // Массив данных, получаемых из DataTable
        TArray<FSquareComponentData*> lPointersToData;

        // Контекст для определения в случае ошибки (см. UDataTable::GetAllRows)
        FString lContext = "CreateGeneratedSquareComponents";

        // Получить массив данных из DataTable
        iSquareComponentTable->GetAllRows<FSquareComponentData>(lContext, lPointersToData);

        CreateGeneratedSquareComponents(lPointersToData);
    }
}
//--------------------------------------------------------------------------------------



/* ---   Stage   --- */

void ASquareGenerator::AddGeneratedSquares(const int32& iAddOnX, UDataTable* iSquareComponentTable)
{
    NumberAlongAxes += FIndex2D(iAddOnX, 0);

    // Создание недостающих элементов массива
    TDArraySquares.SetNum(NumberAlongAxes);

    for (int32 x = NumberAlongAxes.X - iAddOnX; x < NumberAlongAxes.X; ++x)
    {
        for (int32 y = 0; y < NumberAlongAxes.Y; ++y)
        {
            // Создание Клетки и добавление её в массив по соответствующему индексу
            TDArraySquares.SetByIndex(CreateSquare(FIndex2D(x, y)), x, y);
        }
    }

    CreateGeneratedSquareComponents(iSquareComponentTable);

    CreatStageTrigger();
}
//--------------------------------------------------------------------------------------
