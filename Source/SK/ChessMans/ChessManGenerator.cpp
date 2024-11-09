//

// Base:
#include "ChessManGenerator.h"

// UE:
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"

// Interaction:
#include "ChessMan.h"
#include "SK/ChessBoard/Square.h"
#include "SK/Core/SK_Character.h"
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
// Warning: Следует переделать как шаблонные функции универсального Генератора

void AChessManGenerator::ReGenerate()
{
    // Перезапуск Фигур Игроков
    DeleteAllPlayers();
    CreateGeneratedPlayers();

    // Перезапуск Шахматных фигур по контролем ИИ
    DeleteAllChessMans();
    CreateGeneratedChessMans();
}

void AChessManGenerator::DeleteAllPlayers()
{
    // Удаление всех Фигур Игроков, созданных Генератором
    for (auto& lPlayers : GetAllActors<ASK_Character>(VerificationTag))
    {
        lPlayers->Destroy();
    }

    // Очистка массивов
    AllPlayers.Empty(1);
}

void AChessManGenerator::DeleteAllChessMans()
{
    // Удаление всех Шахматных фигур, созданных Генератором
    for (auto& lSquare : GetAllActors<AChessMan>(VerificationTag))
    {
        lSquare->Destroy();
    }

    // Очистка массивов
    AllChessMans.Empty();
    AllAvailableChessMan.Empty();
}
//--------------------------------------------------------------------------------------



/* ---   Generator | Players   --- */
// Warning: Следует переделать как шаблонные функции универсального Генератора

void AChessManGenerator::CreateGeneratedPlayers()
{
    if (PlayersTable)
    {
        // Массив данных, получаемых из DataTable
        TArray<FPlayerData*> lPlayersData;

        // Контекст для определения в случае ошибки (см. UDataTable::GetAllRows)
        FString lContext = "CreateGeneratedPlayers";

        // Получить массив данных из DataTable
        PlayersTable->GetAllRows<FPlayerData>(lContext, lPlayersData);

        // Создать Шахматную фигуру согласно данным
        for (auto& lData : lPlayersData)
        {
            CreatePlayer(lData->Type, lData->Position);

            // Warning: Требуется проверка клетки на доступность (свободна ли она)
        }
    }
}

void AChessManGenerator::CreatePlayer(const TSubclassOf<ASK_Character> iType, const FIndex2D& iXY)
{
    // Поочерёдная проверка валидности указателей и индексов массива
    if (iType
        && PointerToAllSquares
        && PointerToAllSquares->IsValidIndex(iXY)
        && AllPlayers.Num() <= EAutoReceiveInput::Player7)
    {
        // Получение указателя на указанную клетку
        const ASquare* lSquare = PointerToAllSquares->GetByIndex(iXY);

        if (lSquare)
        {
            // Создать Фигуру
            ASK_Character* lNewPlayer = GetWorld()->SpawnActor<ASK_Character>(
                iType.Get(),                 // Тип фигуры
                lSquare->GetActorLocation(), // Локация клетки
                FRotator::ZeroRotator);      // Без изменения ротации

            // Тег-маркировка Фигуры.
            // Необходим для удаления только Генерируемых Фигур
            lNewPlayer->Tags.Add(VerificationTag);

            AllPlayers.Add(lNewPlayer);

            // Автоподхват Игрока
            lNewPlayer->AutoPossessPlayer = EAutoReceiveInput::Type(uint8(AllPlayers.Num()));
            // Warning: Данный способ дважды вызывает PossessedBy(*)
        }
    }
    else if (!iType)
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': iType is NOT"),
            *GetNameSafe(this));
    }
    else if (!PointerToAllSquares)
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': Pointer To AllSquares is NOT"),
            *GetNameSafe(this));
    }
    else if (!PointerToAllSquares->IsValidIndex(iXY))
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s': PointerToAllSquares [%d, %d] is NOT"),
            *GetNameSafe(this), iXY.X, iXY.Y);
    }
    else if (AllPlayers.Num() > EAutoReceiveInput::Player7)
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s': Player limit (8 Players)"),
            *GetNameSafe(this));
    }
}
//--------------------------------------------------------------------------------------



/* ---   Generator | ChessMan   --- */
// Warning: Следует переделать как шаблонные функции универсального Генератора

void AChessManGenerator::CreateGeneratedChessMans()
{
    if (ChessMansTable)
    {
        // Массив данных, получаемых из DataTable
        TArray<FChessManData*> lChessManData;

        // Контекст для определения в случае ошибки (см. UDataTable::GetAllRows)
        FString lContext = "CreateGeneratedChessMans";

        // Получить массив данных из DataTable
        ChessMansTable->GetAllRows<FChessManData>(lContext, lChessManData);

        // Создать Шахматную фигуру согласно данным
        for (auto& lData : lChessManData)
        {
            CreateChessMan(*lData);

            // Warning: Требуется проверка клетки на доступность (свободна ли она)
        }
    }
}

void AChessManGenerator::CreateChessMan(const FChessManData& iData)
{
    // Поочерёдная проверка валидности указателей и индексов массива
    if (ChessManType[uint8(iData.Type)]
        && PointerToAllSquares
        && PointerToAllSquares->IsValidIndex(iData.Position))
    {
        // Получение указателя на указанную клетку
        ASquare* lSquare = PointerToAllSquares->GetByIndex(iData.Position);

        if (lSquare)
        {
            // Указать, что Клетка занята
            lSquare->OccupySquare(EWarringPartiesType::Black);

            // Создать Шахматную фигуру
            AChessMan* lNewChessMan = GetWorld()->SpawnActor<AChessMan>(
                ChessManType[uint8(iData.Type)].Get(), // Тип фигуры
                lSquare->GetActorLocation(),         // Локация клетки
                FRotator::ZeroRotator);              // Без изменения ротации

            // Тег-маркировка Фигуры.
            // Необходим для удаления только Генерируемых Фигур
            lNewChessMan->Tags.Add(VerificationTag);

            lNewChessMan->CurrentData = iData;
            lNewChessMan->SetCurrentSquare(lSquare);

            AllChessMans.Add(lNewChessMan);
        }
    }
    else if (!ChessManType[uint8(iData.Type)])
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': ChessManType [%d] is NOT"),
            *GetNameSafe(this), iData.Type);
    }
    else if (!PointerToAllSquares)
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': Pointer To AllSquares is NOT"),
            *GetNameSafe(this));
    }
    else if (!PointerToAllSquares->IsValidIndex(iData.Position.X))
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s': PointerToAllSquares [%d, Y] is NOT"),
            *GetNameSafe(this), iData.Position.X);
    }
    else if (!PointerToAllSquares->IsValidIndex(iData.Position))
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s': PointerToAllSquares [%d, %d] is NOT"),
            *GetNameSafe(this), iData.Position.X, iData.Position.Y);
    }
}
//--------------------------------------------------------------------------------------



/* ---   Data   --- */

void AChessManGenerator::SetPointerToAllSquares(FSquareArray2D* iSquares)
{
    PointerToAllSquares = iSquares;
}

TArray<ASK_Character*>* AChessManGenerator::GetPointerToAllPlayers()
{
    return &AllPlayers;
}

TArray<AChessMan*>* AChessManGenerator::GetPointerToAllChessMans()
{
    return &AllChessMans;
}

TArray<AChessMan*>* AChessManGenerator::GetPointerToAllAvailableChessMans()
{
    return &AllAvailableChessMan;
}

void AChessManGenerator::UpdateAllAvailableChessMan()
{
    AllAvailableChessMan.Empty();

    if (AllChessMans.IsValidIndex(0))
    {
        AChessMan* lChessMan = nullptr;

        for (auto& lData : AllChessMans)
        {
            lChessMan = GetAvailableChessMan(lData, PointerToAllSquares);

            if (lChessMan)
            {
                AllAvailableChessMan.Add(lChessMan);
            }
        }
    }
}
//--------------------------------------------------------------------------------------
