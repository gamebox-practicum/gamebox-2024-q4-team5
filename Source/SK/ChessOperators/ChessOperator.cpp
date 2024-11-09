// 

// Base:
#include "ChessOperator.h"

// Tools:
#include "SK/Tools/MyRandom.h"

// UE:
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"

// Interaction:
#include "SK/ChessBoard/SquareGenerator.h"
#include "SK/ChessMans/ChessManGenerator.h"
#include "SK/ChessMans/ChessMan.h"
#include "SK/ChessMans/ChessManStruct.h"
#include "SK/ChessBoard/Square.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values
AChessOperator::AChessOperator()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Предварительно

}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

// Called when the game starts or when spawned
void AChessOperator::BeginPlay()
{
    Super::BeginPlay();

    PrimitiveAI_Init();

    TimerInit_MovesSequence(); // Первый ход ограничен по времени

    OnPlayersMove.AddDynamic(this, &AChessOperator::PlayerMovesSequence);
}

void AChessOperator::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    //ReGenerate();
}
//--------------------------------------------------------------------------------------



/* ---   Re Generate   --- */

void AChessOperator::ReGenerate()
{
    UpdateCurrentSquareGenerator();

    UpdateCurrentChessManGenerator();
}
//--------------------------------------------------------------------------------------



/* ---   Generators   --- */

template<typename T>
T* AChessOperator::GetFirstActor()
{
    T* lResult = nullptr;
    TArray<AActor*> lResultActors;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), T::StaticClass(), lResultActors);

    if (lResultActors.IsValidIndex(0))
    {
        lResult = Cast<T>(lResultActors[0]);
    }

    return lResult;
}
//--------------------------------------------------------------------------------------



/* ---   Generators | Square Generator   --- */

void AChessOperator::UpdateCurrentSquareGenerator()
{
    if (GetCurrentSquareGenerator())
    {
        CurrentSquareGenerator->NumberAlongAxes = NumberAlongAxes;

        CurrentSquareGenerator->ReGenerate();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': Current Square Generator is NOT"),
            *GetNameSafe(this));
    }
}

ASquareGenerator* AChessOperator::GetCurrentSquareGenerator()
{
    if (!CurrentSquareGenerator)
    {
        CurrentSquareGenerator = GetFirstSquareGenerator();
    }

    if (CurrentSquareGenerator)
    {
        return CurrentSquareGenerator;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': CurrentSquareGenerator is NOT"),
            *GetNameSafe(this));
    }

    return nullptr;
}

ASquareGenerator* AChessOperator::GetFirstSquareGenerator()
{
    return GetFirstActor<ASquareGenerator>();
}
//--------------------------------------------------------------------------------------



/* ---   Generators | ChessMan Generator   --- */

void AChessOperator::UpdateCurrentChessManGenerator()
{
    if (GetCurrentChessManGenerator())
    {
        CurrentChessManGenerator->PlayersTable = PlayersTable;
        CurrentChessManGenerator->ChessMansTable = ChessMansTable;

        CurrentChessManGenerator->ReGenerate();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': CurrentChessManGenerator is NOT"),
            *GetNameSafe(this));
    }
}

AChessManGenerator* AChessOperator::GetCurrentChessManGenerator()
{
    if (!CurrentChessManGenerator)
    {
        CurrentChessManGenerator = GetFirstChessManGenerator();
    }

    if (CurrentChessManGenerator)
    {
        if (CurrentSquareGenerator)
        {
            CurrentChessManGenerator->SetPointerToAllSquares(
                CurrentSquareGenerator->GetPointerToAllSquares());
        }

        return CurrentChessManGenerator;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': CurrentChessManGenerator is NOT"),
            *GetNameSafe(this));
    }

    return nullptr;
}

AChessManGenerator* AChessOperator::GetFirstChessManGenerator()
{
    return GetFirstActor<AChessManGenerator>();
}
//--------------------------------------------------------------------------------------



/* ---   Player Moves Sequence   --- */

void AChessOperator::PlayerMovesSequence(bool bIsPlayersMove)
{
    if (!bIsPlayersMove
        && GetCurrentChessManGenerator())
    {
        CurrentChessManGenerator->UpdateAllAvailableChessMan();

        PlayPrimitiveAI();

        OnPlayersMove.Broadcast(true);
    }
    else if (!CurrentChessManGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': CurrentChessManGenerator is NOT"),
            *GetNameSafe(this));
    }
    else
    {
        TimerInit_MovesSequence();
    }
}

void AChessOperator::TimerInit_MovesSequence()
{
    GetWorldTimerManager().SetTimer(
        Timer_MovesSequence,
        this,
        &AChessOperator::TimerAction_OperatorMove,
        MoveLimitTime,
        false);
}

void AChessOperator::TimerAction_OperatorMove()
{
    OnPlayersMove.Broadcast(false);
}
//--------------------------------------------------------------------------------------



/* ---   Primitive AI   --- */

void AChessOperator::PrimitiveAI_Init()
{
    // Получить массив из доступных Шахматных фигур
    PointerToAllAvailableChessMans = CurrentChessManGenerator->GetPointerToAllAvailableChessMans();

    // Получить Двумерный массив указателей на Клетки
    PointerToAllSquares = CurrentSquareGenerator->GetPointerToAllSquares();
}

void AChessOperator::PlayPrimitiveAI()
{
    // Рандомный индекс массива
    int32 lRandomNumber;

    // Рандомно выбранная Шахматная фигура
    AChessMan* lSelectedChessMan = nullptr;

    // Массив из доступных ходов данной Шахматной фигуры
    TArray<FIndex2D>* lAllIndex2D = nullptr;

    // Текущая локация Игрока (без оси Z)
    FVector2D lPlayerLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());

    // Проверяемая выбранная позиция
    FVector2D lSelectedLocation;

    // Клетка, на которую пал ход
    ASquare* lSelectedSquare = nullptr;

    // Расстояние между игроком и выбранной локацией
    float F1 = 0.f;
    // Расстояние между игроком и проверяемой локацией
    float F2 = 0.f;

    float test = 0.f;

    // Счётчик для выхода из цикла (дабы не завис)
    uint8 Counter = 3;

    do
    {
        // Получение рандомного значения индекса из массива доступных Шахматных фигур
        lRandomNumber = GetRandom(PointerToAllAvailableChessMans->Num() - 1);

        // Получение указателя на выбранную Шахматную фигуру
        lSelectedChessMan = (*PointerToAllAvailableChessMans)[lRandomNumber];

        // Получение массива доступных ходов выбранной Шахматной фигуры
        lAllIndex2D = &lSelectedChessMan->CurrentData.AvailablePositions;

        // Предварительно ровна расстоянию между игроком и выбранной Шахматной фигурой
        F1 = (lPlayerLocation - FVector2D(lSelectedChessMan->GetActorLocation())).Size();

        // Предварительный сброс
        F2 = 0.f;

        // Получение хода, ближайшего к игроку с проверкой текущей позиции
        for (auto& Data : *lAllIndex2D)
        {
            // Расчёт расстояния между игроком и проверяемой локацией
            F2 = (lPlayerLocation - FVector2D(PointerToAllSquares->GetByIndex(Data)->GetActorLocation())).Size();

            if (F1 > F2)
            {
                lSelectedSquare = PointerToAllSquares->GetByIndex(Data);
                F1 = (lPlayerLocation - FVector2D(lSelectedSquare->GetActorLocation())).Size();
            }
        }

        --Counter;

    } while (F1 == (lPlayerLocation - FVector2D(lSelectedChessMan->GetActorLocation())).Size() && Counter);

    // Переместить выбранную фигуру на выбранную клетку
    lSelectedChessMan->MoveToSquare(lSelectedSquare);
}
//--------------------------------------------------------------------------------------
