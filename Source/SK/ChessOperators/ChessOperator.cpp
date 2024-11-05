// 

// Base:
#include "ChessOperator.h"

// UE:
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"

// Interaction:
#include "SK/ChessBoard/SquareGenerator.h"
#include "SK/ChessMans/ChessManGenerator.h"
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
    CheckCurrentSquareGenerator();

    CheckCurrentChessManGenerator();
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



/* ---   Generators : Square Generator   --- */

void AChessOperator::CheckCurrentSquareGenerator()
{
    if (!CurrentSquareGenerator)
    {
        CurrentSquareGenerator = GetFirstSquareGenerator();
    }

    if (CurrentSquareGenerator)
    {
        UpdateSquareGeneratorData();
        CurrentSquareGenerator->ReGenerate();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': Current Square Generator is NOT"), *GetNameSafe(this));
    }
}

ASquareGenerator* AChessOperator::GetFirstSquareGenerator()
{
    return GetFirstActor<ASquareGenerator>();
}

void AChessOperator::UpdateSquareGeneratorData()
{
    CurrentSquareGenerator->NumberAlongAxes = NumberAlongAxes;
}
//--------------------------------------------------------------------------------------



/* ---   Generators : ChessMan Generator   --- */

void AChessOperator::CheckCurrentChessManGenerator()
{
    if (!CurrentChessManGenerator)
    {
        CurrentChessManGenerator = GetFirstChessManGenerator();
    }

    if (CurrentChessManGenerator)
    {
        UpdateChessManGeneratorData();
        CurrentChessManGenerator->ReGenerate();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': Current ChessMan Generator is NOT"), *GetNameSafe(this));
    }
}

AChessManGenerator* AChessOperator::GetFirstChessManGenerator()
{
    return GetFirstActor<AChessManGenerator>();
}

void AChessOperator::UpdateChessManGeneratorData()
{
    if (CurrentSquareGenerator)
    {
        CurrentChessManGenerator->SetPointerToAllSquares(
            CurrentSquareGenerator->GetPointerToAllSquares());

        CurrentChessManGenerator->ChessMansTable = ChessMansTable;
    }
}
//--------------------------------------------------------------------------------------
