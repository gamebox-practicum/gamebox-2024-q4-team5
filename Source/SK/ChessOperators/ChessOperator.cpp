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
#include "SK/Tools/SKUtils.h"
#include "SK/Tools/Chess_AI/ChessAILibrary.h"
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

    OperatorDataPreInit();

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
    OperatorDataPreInit();

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

void AChessOperator::OperatorDataPreInit()
{
    // Контекст для определения в случае ошибки (см. UDataTable::GetAllRows)
    FString lContext = "OperatorDataPreInit";

    // Получить массив данных из DataTable
    OperatorTable->GetAllRows<FChessOperatorData>(lContext, CurrentOperatorData);
}
//--------------------------------------------------------------------------------------



/* ---   Generators | Square Generator   --- */

void AChessOperator::UpdateCurrentSquareGenerator()
{
    if (GetCurrentSquareGenerator())
    {
        // Приоритет размера по оси X у "нулевых" данных Таблицы "OperatorTable"
        if (CurrentOperatorData[0]->AddOnX)
        {
            CurrentSquareGenerator->NumberAlongAxes = FIndex2D(CurrentOperatorData[0]->AddOnX, NumberAlongAxes.Y);
        }
        else
        {
            CurrentSquareGenerator->NumberAlongAxes = NumberAlongAxes;
        }

        // Передать "нулевую" Таблицу генерации 
        CurrentSquareGenerator->SquareComponentTable = CurrentOperatorData[0]->SquareComponentTable;

        // Передать указатель на самого себя
        CurrentSquareGenerator->SetPointerToOperator(this);

        // Обновить генератор
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
        // Передать "нулевые" Таблицы генерации 
        CurrentChessManGenerator->PlayersTable = CurrentOperatorData[0]->PlayersTable;
        CurrentChessManGenerator->ChessMansTable = CurrentOperatorData[0]->ChessMansTable;

        // Обновить генератор
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

            if (bSkipOperatorTurn)
            {
                bSkipOperatorTurn = false;

                // Задержка передачи управления Игроку, для корректного срабатывания
                GetWorldTimerManager().SetTimer(
                    Timer_MovesSequence,
                    this,
                    &AChessOperator::TimerAction_PlayersMove,
                    0.5f,
                    false);
            }
            else
            {
                PlayPrimitiveAI();
            }
        }
        else if (!CurrentChessManGenerator)
        {
            UE_LOG(LogTemp, Error, TEXT("'%s': CurrentChessManGenerator is NOT"),
                *GetNameSafe(this));
        }
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

void AChessOperator::TimerAction_PlayersMove() const
{
    OnPlayersMove.Broadcast(true);
}
//--------------------------------------------------------------------------------------



/* ---   Stage   --- */

void AChessOperator::ToNextStage()
{
    bSkipOperatorTurn = true;

    ++CurrentStageNum;

    if (CurrentOperatorData.Num() > CurrentStageNum)
    {
        CurrentSquareGenerator->AddGeneratedSquares(
            CurrentOperatorData[CurrentStageNum]->AddOnX,
            CurrentOperatorData[CurrentStageNum]->SquareComponentTable);

        CurrentChessManGenerator->AddGeneratedChessMans(
            CurrentOperatorData[CurrentStageNum]->PlayersTable,
            CurrentOperatorData[CurrentStageNum]->ChessMansTable);
    }
    else
    {
        // Warning: Требуется заменить на триггер завершения Уровня
        UE_LOG(LogTemp, Warning, TEXT("'%s': EndGame"),
            *GetNameSafe(this));
    }
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
    //почему то может вызваться повторно до завершения рассчетов
    if (StepIsCalculatedNow)
    {
        UE_LOG(LogTemp, Error,
            TEXT("AChessOperator::PlayPrimitiveAI:: the function is called again before the calculation is completed"));
        return;
    }
    StepIsCalculatedNow = true;

    if (!IsValid(ChessBoardInfo))
    {
        ChessBoardInfo = NewObject<UChessBoardInfo>(this);
    }

    //инициализируем обьект с данными фигур
    ChessBoardInfo->Init(CurrentSquareGenerator->NumberAlongAxes.Y, CurrentSquareGenerator->NumberAlongAxes.X);

    //размещаем все фигуры

    for (auto figure : *PointerToAllChessMans)
    {
        //figure почему то может быть невалидной
        if (!IsValid(figure))
        {
            UE_LOG(LogTemp, Error, TEXT("AChessOperator::PlayPrimitiveAI:: 'figure' is invalid"));
            //StepIsCalculatedNow = false;
            //OnPlayersMove.Broadcast(true);
            //return;
            continue;
        }
        auto chessPiece = SKUtils::ConstructChessPiece(figure->CurrentData.Type,
            PIECE_COLOR::BLACK, this);
        if (chessPiece)
        {
            ChessBoardInfo->Set(figure->CurrentData.Position.Y, figure->CurrentData.Position.X, chessPiece);
        }
    }

    //! сейчас считает только одну белую фигуру.
    // нужна либо список AChessMan* белих фигур (аналогично PointerToAllChessMans)
    // либо переменная с типом фигуры в SK_Character.h
    if (AllPlayers->Num() > 0)
    {
        auto chessPiece = SKUtils::ConstructChessPiece(EChessManType::King,
            PIECE_COLOR::WHITE, this);
        if (chessPiece)
        {
            auto pos = (*AllPlayers)[0]->GetCurrentPosition();
            ChessBoardInfo->Set(pos.Y, pos.X, chessPiece);
        }
    }

    FOnStepCalculatedSignature OnStepCalculated;
    OnStepCalculated.BindDynamic(this, &AChessOperator::OnBlackStepCalculated);
    UChessAILibrary::GetNextStepAsync(ChessBoardInfo, PIECE_COLOR::BLACK, 5, OnStepCalculated);

}

void AChessOperator::OnBlackStepCalculated(FChessPieceStep Step)
{
    StepIsCalculatedNow = false;

    //поиск фигуры по индексу клетки
    auto figure = (*PointerToAllChessMans).FindByPredicate([Step](AChessMan* m)
        {
            if (IsValid(m))
            {
                return (m->CurrentData.Position.Y == Step.PreviousPosition.Y) &&
                    (m->CurrentData.Position.X == Step.PreviousPosition.X);
            }
            return false;
        });

    //фигуры почему то иногда двигаются/умирают во время рассчета хода
    if (!figure)
    {
        UE_LOG(LogTemp, Error,
            TEXT("AChessOperator::PlayPrimitiveAI:: figures moved/removed while calculated"));
        OnPlayersMove.Broadcast(true);
        return;
    }

    //индекс почему то иногда выходит за пределы массива PointerToAllSquares
    //(ее размер всегда равен CurrentSquareGenerator->NumberAlongAxes?)
    if (!(PointerToAllSquares->IsValidIndex(Step.NewPosition.X, Step.NewPosition.Y)))
    {
        UE_LOG(LogTemp, Error,
            TEXT("AChessOperator::PlayPrimitiveAI:: invalid PointerToAllSquares index:{%d, %d}"),
            Step.NewPosition.X, Step.NewPosition.Y);
        OnPlayersMove.Broadcast(true);
        return;
    }

    // Переместить выбранную фигуру на выбранную клетку

    ASquare* target = PointerToAllSquares->
        GetByIndex(FIndex2D { Step.NewPosition.X, Step.NewPosition.Y });
    //на всякий случай
    if (!IsValid(target))
    {
        UE_LOG(LogTemp, Error, TEXT("AChessOperator::PlayPrimitiveAI:: invalid target square"));
        OnPlayersMove.Broadcast(true);
        return;
    }
    (*figure)->MoveToSquare(target);

    OnPlayersMove.Broadcast(true);
}
//--------------------------------------------------------------------------------------
