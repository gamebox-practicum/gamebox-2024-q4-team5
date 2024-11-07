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
    // Warning: Convert to Template

    // Удаление всех Фигур Игроков, созданных Генератором
    for (auto& lPlayers : GetAllPlayers())
    {
        lPlayers->Destroy();
    }

    // Очистка массивов
    AllPlayers.Empty(1);
}

TArray<ASK_Character*> AChessManGenerator::GetAllPlayers()
{
    // Warning: Convert to Template

    TArray<ASK_Character*> lResult;
    TArray<AActor*> lResultActors;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASK_Character::StaticClass(), lResultActors);

    if (lResultActors.IsValidIndex(0))
    {
        bool bCheckTags = false;

        for (AActor* lData : lResultActors)
        {
            ASK_Character* lCurrentPlayers = Cast<ASK_Character>(lData);

            for (FName lTags : lCurrentPlayers->Tags)
            {
                if (lTags == VerificationTag)
                {
                    bCheckTags = true;
                }
            }

            if (bCheckTags)
            {
                lResult.Add(lCurrentPlayers);
            }
        }
    }

    return lResult;
}

void AChessManGenerator::DeleteAllChessMans()
{
    // Warning: Convert to Template

    // Удаление всех Шахматных фигур, созданных Генератором
    for (auto& lSquare : GetAllChessMan())
    {
        lSquare->Destroy();
    }

    // Очистка массивов
    AllChessMans.Empty();
    AllAvailableChessMan.Empty();
}

TArray<AChessMan*> AChessManGenerator::GetAllChessMan()
{
    // Warning: Convert to Template

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



/* ---   Generator | Players   --- */
// Warning: Следует переделать как шаблонные функции универсального Генератора

void AChessManGenerator::CreateGeneratedPlayers()
{
    // Warning: Convert to Template

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
    // Warning: Convert to Template

    // Поочерёдная проверка валидности указателей и индексов массива
    if (iType
        && PointerToAllSquares
        && PointerToAllSquares->IsValidIndex(iXY.X)
        && (*PointerToAllSquares)[iXY.X].IsValidIndex(iXY.Y)
        && AllPlayers.Num() <= EAutoReceiveInput::Player7)
    {
        // Получение указателя на указанную клетку
        const ASquare* lSquare = ((*PointerToAllSquares)[iXY.X])[iXY.Y];

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
    else if (!PointerToAllSquares->IsValidIndex(iXY.X))
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s': PointerToAllSquares [%d, Y] is NOT"),
            *GetNameSafe(this), iXY.X);
    }
    else if (!(*PointerToAllSquares)[iXY.X].IsValidIndex(iXY.Y))
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s': PointerToAllSquares [X, %d] is NOT"),
            *GetNameSafe(this), iXY.Y);
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
    // Warning: Convert to Template

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
    // Warning: Convert to Template

    // Поочерёдная проверка валидности указателей и индексов массива
    if (PointerToAllSquares
        && PointerToAllSquares->IsValidIndex(iData.Position.X)
        && (*PointerToAllSquares)[iData.Position.X].IsValidIndex(iData.Position.Y))
    {
        // Получение указателя на указанную клетку
        ASquare* lSquare = ((*PointerToAllSquares)[iData.Position.X])[iData.Position.Y];

        if (lSquare)
        {
            // Указать, что Клетка занята
            lSquare->OccupySquare(EWarringPartiesType::Black);

            // Создать Шахматную фигуру
            AChessMan* lNewChessMan = GetWorld()->SpawnActor<AChessMan>(
                SquareType[uint8(iData.Type)].Get(), // Тип фигуры
                lSquare->GetActorLocation(),    // Локация клетки
                FRotator::ZeroRotator);         // Без изменения ротации

            // Тег-маркировка Фигуры.
            // Необходим для удаления только Генерируемых Фигур
            lNewChessMan->Tags.Add(VerificationTag);

            lNewChessMan->CurrentData = iData;
            lNewChessMan->SetCurrentSquare(lSquare);

            AllChessMans.Add(lNewChessMan);
        }
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
    else if (!(*PointerToAllSquares)[iData.Position.X].IsValidIndex(iData.Position.Y))
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s': PointerToAllSquares [X, %d] is NOT"),
            *GetNameSafe(this), iData.Position.Y);
    }
}
//--------------------------------------------------------------------------------------



/* ---   Data   --- */

void AChessManGenerator::SetPointerToAllSquares(const TArray<TArray<ASquare*>>* iSquares)
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
