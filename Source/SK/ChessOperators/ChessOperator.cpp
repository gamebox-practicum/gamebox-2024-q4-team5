//

// Base:
#include "ChessOperator.h"

// Tools:
#include "SK/Tools/MyRandom.h"

// UE:
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"

// Interaction:
#include "SK/Core/SK_Character.h"
#include "SK/ChessBoard/SquareGenerator.h"
#include "SK/ChessBoard/Square.h"
#include "SK/ChessMans/ChessManGenerator.h"
#include "SK/ChessMans/ChessMan.h"
#include "SK/ChessMans/ChessManStruct.h"
#include "SK/ChessBoard/Square.h"
#include "SK/Tools/SKUtils.h"
#include "SK/Tools/Chess_AI/ChessBoardInfo.h"
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
    if (!bIsPlayersMove)
    {
        if (GetCurrentChessManGenerator())
        {
            CurrentChessManGenerator->UpdateAllAvailableChessMan();

            PlayPrimitiveAI();
        }
        else if (!CurrentChessManGenerator)
        {
            UE_LOG(LogTemp, Error, TEXT("'%s': CurrentChessManGenerator is NOT"),
                *GetNameSafe(this));
        }

        //OnPlayersMove.Broadcast(true);
    }
    else
    {
        TimerInit_MovesSequence();
    }
}

void AChessOperator::StopTimer_MovesSequence()
{
    GetWorldTimerManager().ClearTimer(Timer_MovesSequence);
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

void AChessOperator::TimerAction_OperatorMove() const
{
    OnPlayersMove.Broadcast(false);
}
//--------------------------------------------------------------------------------------



/* ---   Primitive AI   --- */

void AChessOperator::PrimitiveAI_Init()
{
    // Получить массив из всех Шахматных фигур
    PointerToAllChessMans = CurrentChessManGenerator->GetPointerToAllChessMans();

    // Получить массив из доступных Шахматных фигур
    PointerToAllAvailableChessMans = CurrentChessManGenerator->GetPointerToAllAvailableChessMans();

    // Получить все фигуры игроков/игрока
    AllPlayers = CurrentChessManGenerator->GetPointerToAllPlayers();

    // Получить Двумерный массив указателей на Клетки
    PointerToAllSquares = CurrentSquareGenerator->GetPointerToAllSquares();
}

void AChessOperator::PlayPrimitiveAI()
{
    if(!IsValid(ChessBoardInfo))
    {
        ChessBoardInfo = NewObject<UChessBoardInfo>(this);
    }

    //инициализируем обьект с данными фигур
    ChessBoardInfo->Init(CurrentSquareGenerator->NumberAlongAxes.Y, CurrentSquareGenerator->NumberAlongAxes.X);

    //размещаем все фигуры
    auto figures = *PointerToAllAvailableChessMans;

    for(auto figure : figures)
    {
        auto chessPiece = SKUtils::ConstructChessPiece(figure->CurrentData.Type,
            PIECE_COLOR::BLACK, this);
        if(chessPiece)
        {
            ChessBoardInfo->Set(figure->CurrentData.Position.Y, figure->CurrentData.Position.X, chessPiece);
        }
    }


    // Переместить выбранную фигуру на выбранную клетку
    //lSelectedChessMan->MoveToSquare(lSelectedSquare);
}
//--------------------------------------------------------------------------------------
