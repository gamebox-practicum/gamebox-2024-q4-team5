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

    OnPlayersMove.AddDynamic(this, &AChessOperator::PlayersMove);
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



/* ---   Delegate   --- */

void AChessOperator::PlayersMove(bool bIsPlayersMove)
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
}
//--------------------------------------------------------------------------------------



/* ---   Primitive AI   --- */

void AChessOperator::PlayPrimitiveAI()
{
    // Получить массив из доступных Шахматных фигур
    TArray<AChessMan*>* lAllChessMans = CurrentChessManGenerator->GetPointerToAllAvailableChessMans();

    // Рандомный выбор Шахматной фигуры
    int32 lNumber = GetRandom(lAllChessMans->Num() - 1);
    AChessMan* lSelectedChessMan = (*lAllChessMans)[lNumber];

    //UE_LOG(LogTemp, Error, TEXT("'%s': %d is %s"),
    //    *GetNameSafe(this), lNumber, *GetNameSafe(lSelectedChessMan));



    // Получить массив из доступных ходов данной Шахматной фигуры
    TArray<FIndex2D>& lAllIndex2D = lSelectedChessMan->CurrentData.AvailablePositions;

    // Рандомный выбор хода выбранной Шахматной фигуры
    lNumber = GetRandom(lAllIndex2D.Num() - 1);
    FIndex2D lSelectedIndex2D = lAllIndex2D[lNumber];



    // Получить клетку, на которую пал ход
    TArray<TArray<ASquare*>>* lAllSquares = CurrentSquareGenerator->GetPointerToAllSquares();
    ASquare* lSelectedSquare = (*lAllSquares)[lSelectedIndex2D.X][lSelectedIndex2D.Y];

    //UE_LOG(LogTemp, Error, TEXT("'%s': %d is %s"),
    //    *GetNameSafe(this), lNumber, *GetNameSafe(lSelectedSquare));

    // Переместить выбранную фигуру на выбранную клетку
    lSelectedChessMan->MoveToSquare(lSelectedSquare);
}
//--------------------------------------------------------------------------------------
