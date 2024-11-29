//

// Base:
#include "ChessManGenerator.h"

// UE:
#include "Engine/DataTable.h"

// Interaction:
#include "ChessMan.h"
#include "SK/ChessBoard/Square.h"
#include "SK/ChessOperators/ChessOperator.h"
#include "SK/ChessOperators/DealerHand.h"
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
    // Создать Руку дилера, если её нет
    CreateGeneratedDealerHand();

    // Пересоздать Фигур Игроков
    DeleteAllPlayers();
    CreateGeneratedPlayers(PlayersTable);

    // Пересоздать Шахматные фигуры под контролем ИИ
    DeleteAllChessMans();
    CreateGeneratedChessMans(ChessMansTable);
}

void AChessManGenerator::DeleteAllPlayers()
{
    // Удаление всех Фигур Игроков, созданных Генератором
    for (auto& lPlayer : GetAllActors<ASK_Character>(VerificationTag))
    {
        lPlayer->Destroy();
    }

    // Очистка массивов
    AllPlayers.Empty(1);
}

void AChessManGenerator::DeleteAllChessMans()
{
    // Удаление всех Шахматных фигур, созданных Генератором
    for (auto& lChessMan : GetAllActors<AChessMan>(VerificationTag))
    {
        lChessMan->Destroy();
    }

    // Очистка массивов
    AllChessMans.Empty();
    AllAvailableChessMan.Empty();
}

template<class T>
inline T* AChessManGenerator::CreateFigureOnChessboard(const TSubclassOf<AActor>& iType, const FIndex2D& iXY)
{
    // Поочерёдная проверка валидности указателей и индексов массива
    if (iType
        && PointerToAllSquares
        && PointerToAllSquares->IsValidIndex(iXY))
    {
        // Получение указателя на указанную клетку
        const ASquare* lSquare = PointerToAllSquares->GetByIndex(iXY);

        if (lSquare)
        {
            // Параметр создания: Всегда появляется
            FActorSpawnParameters lSpawnParameters;
            lSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            // Создать Фигуру
            T* lNewActor = GetWorld()->SpawnActor<T>(
                iType.Get(),                 // Тип Актора
                lSquare->GetActorLocation(), // Локация клетки
                FRotator::ZeroRotator,       // Без изменения ротации
                lSpawnParameters);

            if (lNewActor)
            {
                // Тег-маркировка генерируемого Актора.
                // Необходим для удаления только Генерируемых Акторов
                lNewActor->Tags.Add(VerificationTag);
            }

            return lNewActor;
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

    return nullptr;
}
//--------------------------------------------------------------------------------------



/* ---   Generator | Dealer Hand   --- */

void AChessManGenerator::CreateGeneratedDealerHand()
{
    if (!CurrentDealerHand)
    {
        // Удаление всех рук Дилера, созданных Генератором
        for (auto& lHand : GetAllActors<ADealerHand>(VerificationTag))
        {
            lHand->Destroy();
        }

        if (DealerHandType)
        {
            CurrentDealerHand = GetWorld()->SpawnActor<ADealerHand>(
                DealerHandType.Get(),   // Тип Руки
                GetActorLocation(),     // Локация текущая Генератора
                FRotator::ZeroRotator); // Без изменения ротации

            if (CurrentDealerHand)
            {
                CurrentDealerHand->SetCurrentChessManGenerator(this);

                // Тег-маркировка Фигуры.
                // Необходим для удаления только Генерируемых Фигур
                CurrentDealerHand->Tags.Add(VerificationTag);
            }
        }
    }
}
//--------------------------------------------------------------------------------------



/* ---   Generator | Players   --- */

void AChessManGenerator::CreateGeneratedPlayers(const TArray<FPlayerData*>& iPlayersData)
{
    // Создать Шахматную фигуру согласно данным
    for (auto& lData : iPlayersData)
    {
        if (AllPlayers.Num() <= EAutoReceiveInput::Player7)
        {
            // Создание Персонажа
            ASK_Character* lNewPlayer = CreateFigureOnChessboard<ASK_Character>(lData->Type, lData->Position);

            if (lNewPlayer)
            {
                // Добавление данных для игрока
                lNewPlayer->SetPointerToOperator(CurrentOperator);
                lNewPlayer->SetCurrentChessManGenerator(this);
                lNewPlayer->SetCurrentDealerHand(CurrentDealerHand);
                lNewPlayer->CurrentPosition = lData->Position;
                lNewPlayer->CurrentSquare = PointerToAllSquares->GetByIndex(lData->Position);

                // Добавление в массив Игроков
                AllPlayers.Add(lNewPlayer);
                lNewPlayer->AutoPossessPlayer = EAutoReceiveInput::Type(uint8(AllPlayers.Num()));

                // Автоподхват Игрока
                lNewPlayer->Initialize();
            }
        }

        // Warning: Требуется проверка клетки на доступность (свободна ли она)
    }
}

ASK_Character* AChessManGenerator::GetFirstPlayer()
{
    ASK_Character* lResult = nullptr;

    for (ASK_Character* Player : AllPlayers)
    {
        if (Player)
        {
            lResult = Player;
            break;
        }
    }

    return lResult;
}

void AChessManGenerator::CreateGeneratedPlayers(UDataTable* iPlayersTable)
{
    if (iPlayersTable)
    {
        // Массив данных, получаемых из DataTable
        TArray<FPlayerData*> lPlayersData;

        // Контекст для определения в случае ошибки (см. UDataTable::GetAllRows)
        FString lContext = "CreateGeneratedPlayers";

        // Получить массив данных из DataTable
        iPlayersTable->GetAllRows<FPlayerData>(lContext, lPlayersData);

        CreateGeneratedPlayers(lPlayersData);
    }
}
//--------------------------------------------------------------------------------------



/* ---   Generator | ChessMan   --- */

void AChessManGenerator::CreateGeneratedChessMans(const TArray<FChessManData*>& iChessMansData)
{
    // Создать Шахматную фигуру согласно данным
    for (auto& lData : iChessMansData)
    {
        CreateChessMansFromData(*lData);
    }
}

void AChessManGenerator::CreateGeneratedChessMans(UDataTable* iChessMansTable)
{
    if (iChessMansTable)
    {
        // Массив данных, получаемых из DataTable
        TArray<FChessManData*> lChessManData;

        // Контекст для определения в случае ошибки (см. UDataTable::GetAllRows)
        FString lContext = "CreateGeneratedChessMans";

        // Получить массив данных из DataTable
        iChessMansTable->GetAllRows<FChessManData>(lContext, lChessManData);

        CreateGeneratedChessMans(lChessManData);
    }
}

void AChessManGenerator::CreateChessMansFromData(const FChessManData& iData)
{
    // Создание Вражеской фигуры
    AChessMan* lNewChessMan = CreateFigureOnChessboard<AChessMan>(ChessManType[uint8(iData.Type)], iData.Position);

    if (lNewChessMan)
    {
        lNewChessMan->CurrentData = iData;
        lNewChessMan->SetCurrentSquare(PointerToAllSquares->GetByIndex(iData.Position));
        lNewChessMan->SetCurrentChessManGenerator(this);
        lNewChessMan->SetPointerToOperator(CurrentOperator);
        lNewChessMan->SetCurrentDealerHand(CurrentDealerHand);
        lNewChessMan->AddActorWorldRotation(FRotator(0.f, -180.f, 0.f));

        AllChessMans.Add(lNewChessMan);

        lNewChessMan->Initialize();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': lNewChessMan is InValid: '%s'"),
            *GetNameSafe(this), *GetNameSafe(lNewChessMan));
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

TArray<FAttackingChessMansData>* AChessManGenerator::GetPointerToAttackingChessMans()
{
    return &AttackingChessMans;
}

void AChessManGenerator::RemovePlayer(ASK_Character* iPlayer)
{
    AllPlayers.Remove(iPlayer);
}

void AChessManGenerator::RemoveChessMan(AChessMan* iChessMan)
{
    AllAvailableChessMan.RemoveSwap(iChessMan);
    AllChessMans.RemoveSwap(iChessMan);
}

void AChessManGenerator::UpdateAllAvailableChessMan()
{
    AllAvailableChessMan.Empty();
    AttackingChessMans.Empty();

    if (AllChessMans.IsValidIndex(0))
    {
        AChessMan* lChessMan = nullptr;

        for (auto& lData : AllChessMans)
        {
            lChessMan = GetAvailableChessMan(lData, PointerToAllSquares, AttackingChessMans);

            if (lChessMan)
            {
                AllAvailableChessMan.Add(lChessMan);
            }
        }
    }
}

void AChessManGenerator::SetPointerToOperator(AChessOperator* iCurrentOperator)
{
    CurrentOperator = iCurrentOperator;
}
//--------------------------------------------------------------------------------------



/* ---   Stage   --- */

void AChessManGenerator::AddGeneratedChessMans(UDataTable* iPlayersTable, UDataTable* iChessMansTable)
{
    CreateGeneratedPlayers(iPlayersTable);
    CreateGeneratedChessMans(iChessMansTable);
}
//--------------------------------------------------------------------------------------
