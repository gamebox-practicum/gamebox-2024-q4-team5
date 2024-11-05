//

// Base:
#include "ChessManGenerator.h"

// UE:
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"

// Interaction:
#include "ChessMan.h"
#include "SK/ChessBoard/Square.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values
AChessManGenerator::AChessManGenerator()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Предварительно

}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

// Called when the game starts or when spawned
void AChessManGenerator::BeginPlay()
{
    Super::BeginPlay();

}

void AChessManGenerator::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    //ReGenerate();
}
//--------------------------------------------------------------------------------------



/* ---   Re Generate   --- */
// PS: Следует переделать как шаблонные функции универсального Генератора

void AChessManGenerator::ReGenerate()
{
    DeleteAllSquares();
    CreateGeneratedChessMan();
}

void AChessManGenerator::DeleteAllSquares()
{
    // Удаление всех Шахматных фигур, созданных Генератором
    for (auto lSquare : GetAllChessMan())
    {
        lSquare->Destroy();
    }

    // Очистка массивов
    AllChessMans.Empty();
    AllAvailableChessMan.Empty();
}

TArray<AChessMan*> AChessManGenerator::GetAllChessMan()
{
    TArray<AChessMan*> lResult;
    TArray<AActor*> lResultActors;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChessMan::StaticClass(), lResultActors);

    if (lResultActors.IsValidIndex(0))
    {
        bool bCheckTags = false;

        for (AActor* lData : lResultActors)
        {
            AChessMan* lCurrentChessMan = Cast<AChessMan>(lData);

            for (FName lTags : lCurrentChessMan->Tags)
            {
                if (lTags == VerificationTag)
                {
                    bCheckTags = true;
                }
            }

            if (bCheckTags)
            {
                lResult.Add(lCurrentChessMan);
            }
        }
    }

    return lResult;
}
//--------------------------------------------------------------------------------------



/* ---   Generator   --- */
// PS: Следует переделать как шаблонные функции универсального Генератора

void AChessManGenerator::CreateGeneratedChessMan()
{
    if (ChessMansTable)
    {
        // Массив данных, получаемых из DataTable
        TArray<FChessManData*> lChessManData;

        // Контекст для определения в случае ошибки (см. UDataTable::GetAllRows)
        FString lContext = "";

        // Получить массив данных из DataTable
        ChessMansTable->GetAllRows<FChessManData>(lContext, lChessManData);

        // Создать Шахматную фигуру согласно данным
        for (auto lData : lChessManData)
        {
            CreateChessMan(lData->Type, lData->Position);

            // Warning: Требуется проверка клетки на доступность (свободна ли она)
        }
    }
}

void AChessManGenerator::CreateChessMan(const EChessManType iType, const FIndex2D& iXY)
{
    // Проверка валидности указателя и его индексов
    if (PointerToAllSquares)
    {
        if (PointerToAllSquares->IsValidIndex(iXY.X))
        {
            if ((*PointerToAllSquares)[iXY.X].IsValidIndex(iXY.Y))
            {
                // Получение указателя на указанную клетку
                const ASquare* lSquare = ((*PointerToAllSquares)[iXY.X])[iXY.Y];

                if (lSquare)
                {
                    // Создать Шахматную фигуру
                    AChessMan* lNewChessMan = GetWorld()->SpawnActor<AChessMan>(
                        SquareType[uint8(iType)].Get(), // Тип фигуры
                        lSquare->GetActorLocation(),    // Локация клетки
                        FRotator::ZeroRotator);         // Без изменения ротации

                    // Тег-маркировка Фигуры.
                    // Необходим для удаления только Генерируемых Фигур
                    lNewChessMan->Tags.Add(VerificationTag);

                    AllChessMans.Add(lNewChessMan);
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("'%s': PointerToAllSquares [X, %d] is NOT"),
                    *GetNameSafe(this), iXY.Y);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("'%s': PointerToAllSquares [%d, Y] is NOT"),
                *GetNameSafe(this), iXY.X);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': Pointer To AllSquares is NOT"), *GetNameSafe(this));
    }
}
//--------------------------------------------------------------------------------------



/* ---   Data   --- */

void AChessManGenerator::SetPointerToAllSquares(const TArray<TArray<ASquare*>>* iSquares)
{
    PointerToAllSquares = iSquares;
}

TArray<AChessMan*>* AChessManGenerator::GetPointerToAllChessMan()
{
    return &AllChessMans;
}

TArray<AChessMan*>* AChessManGenerator::GetPointerToAllAvailableChessMan()
{
    return &AllAvailableChessMan;
}
//--------------------------------------------------------------------------------------
