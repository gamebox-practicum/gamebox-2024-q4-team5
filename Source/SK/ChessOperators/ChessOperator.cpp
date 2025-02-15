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
#include "SK/Core/SK_GameMode.h"
#include "SK/ChessBoard/SquareGenerator.h"
#include "SK/ChessBoard/Square.h"
#include "SK/ChessMans/ChessManGenerator.h"
#include "SK/ChessMans/ChessMan.h"
#include "SK/ChessMans/ChessManStruct.h"
#include "SK/TimeBeacons/TimeBeaconGenerator.h"
#include "SK/TimeBeacons/TimeBeacon.h"

// AI:
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

    OperatorDataPreInit();

    OnPlayersMove.AddUObject(this, &AChessOperator::PlayerMovesSequence);

    SavedDataInit();

    if (CurrentGameInstance)
    {
        if (CurrentGameInstance->bIsNewGame)
        {
            CurrentGameInstance->ClearLevelData();

            if (CurrentGameInstance->IntermediatePlayerPositionY >= 0)
            {
                ASquare* lNewSquare =
                    PointerToAllSquares->GetByIndex(
                        0,
                        CurrentGameInstance->IntermediatePlayerPositionY + CurrentGameInstance->CurrentOffsetPlayersPositionAlongY);

                (*AllPlayers)[0]->SetActorLocation(lNewSquare->GetActorLocation());
                (*AllPlayers)[0]->CurrentPosition = lNewSquare->GetData().PositionNumber;
            }
        }
        else
        {
            CurrentGameInstance->bIsNewGame = true;

            if (CurrentGameInstance->IsGameSaved())
            {
                CurrentChessManGenerator->SetPointerToAllSquares(
                    CurrentSquareGenerator->GetPointerToAllSquares());

                UploadLevelData();
            }
        }
    }
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

    // Очистить и получить массив данных из DataTable
    CurrentOperatorData.Empty();
    OperatorTable->GetAllRows<FChessOperatorData>(lContext, CurrentOperatorData);
    TotalStageNum = CurrentOperatorData.Num();
}

FIndex2D AChessOperator::GetFullNumberAlongAxes()
{
    FIndex2D lResult = NumberAlongAxes;

    if (CurrentOperatorData[0]->AddOnX != 0)
    {
        lResult.X = 0;
    }

    for (auto& lData : CurrentOperatorData)
    {
        lResult.X += lData->AddOnX;
    }

    return lResult;
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

        // Передать указатель на самого себя
        CurrentChessManGenerator->SetPointerToOperator(this);

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



/* ---   Generators | Time Beacon Generator   --- */

void AChessOperator::UpdateCurrentTimeBeaconGenerator(const FVector& iBlockSize)
{
    if (GetCurrentTimeBeaconGenerator())
    {
        if (CurrentTimeBeaconGenerator->BlockSize != iBlockSize)
        {
            // Передать данные для генерации
            CurrentTimeBeaconGenerator->BeaconType = BeaconType;
            CurrentTimeBeaconGenerator->BlockSize = iBlockSize;
            CurrentTimeBeaconGenerator->NumberOfSquaresAlongAxes = GetFullNumberAlongAxes();

            // Обновить генератор
            CurrentTimeBeaconGenerator->ReGenerate();
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': CurrentTimeBeaconGenerator is NOT"),
            *GetNameSafe(this));
    }
}

ATimeBeaconGenerator* AChessOperator::GetCurrentTimeBeaconGenerator()
{
    if (!CurrentTimeBeaconGenerator)
    {
        CurrentTimeBeaconGenerator = GetFirstTimeBeaconGenerator();
    }

    if (CurrentTimeBeaconGenerator)
    {
        CurrentTimeBeaconGenerator->SetTimeForBeacons(MoveLimitTime);

        return CurrentTimeBeaconGenerator;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': CurrentTimeBeaconGenerator is NOT"),
            *GetNameSafe(this));
    }

    return nullptr;
}

ATimeBeaconGenerator* AChessOperator::GetFirstTimeBeaconGenerator()
{
    return GetFirstActor<ATimeBeaconGenerator>();
}
//--------------------------------------------------------------------------------------



/* ---   Player Moves Sequence   --- */

void AChessOperator::PlayerMovesSequence(const bool& ibIsPlayersMove)
{
    if (AllPlayers->Num())
    {
        if (GetCurrentChessManGenerator())
        {
            CurrentChessManGenerator->UpdateAllAvailableChessMan();

            if (!ibIsPlayersMove)
            {
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
            else
            {
                TimerInit_MovesSequence();
                PlayingAttackSound();
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
        StopTimer_MovesSequence();
    }
}

void AChessOperator::StopTimer_MovesSequence()
{
    GetWorldTimerManager().ClearTimer(Timer_MovesSequence);
    CurrentTimeBeaconGenerator->StopTimeBeaconGenerator();
}

void AChessOperator::TimerInit_MovesSequence()
{
    GetWorldTimerManager().SetTimer(
        Timer_MovesSequence,
        this,
        &AChessOperator::TimerAction_OperatorMove,
        MoveLimitTime,
        false);

    CurrentTimeBeaconGenerator->StopTimeBeaconGenerator();
    CurrentTimeBeaconGenerator->PlayTimeBeaconGenerator();
}

void AChessOperator::TimerAction_OperatorMove() const
{
    if (PointerToAllChessMans->Num() != 0)
    {
        OnPlayersMove.Broadcast(false);
    }
    else
    {
        (*AllPlayers)[0]->DeathByDealerHand();

        //for (int32 i = 0; i < AllPlayers->Num(); ++i)
        //{
        //    if ((*AllPlayers)[i])
        //    {
        //        (*AllPlayers)[i]->DeathByDealerHand();
        //    }
        //}
        /* PS: В данном случае нельзя использовать следующий вариант: for (auto& Player : *AllPlayers)
        * При вызове CharacterDeath() происходит изменение массива AllPlayers
        * что вызывает ошибку при таком варианте использования "for".
        *
        * Ошибка "Array has changed during ranged-for iteration!"
        * ("Массив изменился во время итерации ranged-for!")
        */
    }
}

void AChessOperator::TimerAction_PlayersMove() const
{
    OnPlayersMove.Broadcast(true);
}
//--------------------------------------------------------------------------------------



/* ---   Stage   --- */

void AChessOperator::ToNextStage()
{
    ++CurrentStageNum;

    // Проверка НЕ выигрышного этапа
    if (TotalStageNum > CurrentStageNum)
    {
        CurrentSquareGenerator->AddGeneratedSquares(
            CurrentOperatorData[CurrentStageNum]->AddOnX,
            CurrentOperatorData[CurrentStageNum]->SquareComponentTable);

        CurrentChessManGenerator->AddGeneratedChessMans(
            CurrentOperatorData[CurrentStageNum]->PlayersTable,
            CurrentOperatorData[CurrentStageNum]->ChessMansTable);

        // Сохранение данных на новом этапе
        SaveLevelData();

        // Проверка атаки на игрока
        PlayingAttackSound();

        CurrentTimeBeaconGenerator->UpdateStage(CurrentStageNum);

        OnNextStage();
    }
    else
    {
        // Блокировать ход Вражеских фигур, чтобы в конце игры не убивали ГГ
        bSkipOperatorTurn = true;

        CurrentGameInstance->IntermediatePlayerPositionY = (*AllPlayers)[0]->GetCurrentPosition().Y;

        // Если этапы закончились, то завершить игру с победой
        Cast<ASK_GameMode>(GetWorld()->GetAuthGameMode())->SetWinningGame();
    }
}
//--------------------------------------------------------------------------------------



/* ---   Attack   --- */

void AChessOperator::PlayingAttackSound()
{
    if (WalkingSound)
    {
        if (CurrentChessManGenerator->GetPointerToAttackingChessMans()->Num())
        {
            for (auto& lData : *CurrentChessManGenerator->GetPointerToAttackingChessMans())
            {
                if (lData.AttackPosition == (*AllPlayers)[0]->GetCurrentPosition())
                {
                    UGameplayStatics::PlaySoundAtLocation(
                        GetWorld(),
                        WalkingSound,
                        (*AllPlayers)[0]->GetActorLocation());
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s': WalkingSound is NOT"),
            *GetNameSafe(this));
    }
}
//--------------------------------------------------------------------------------------



/* ---   Level Saving   --- */

void AChessOperator::SaveLevelData() const
{
    if (CurrentGameInstance)
    {
        FLevelData lCurrentData;

        /* ---   Chess Operator Data   --- */

        lCurrentData.bIsPlayersMove = bIsPlayersMove;
        lCurrentData.CurrentStageNum = CurrentStageNum;
        lCurrentData.MoveLimitTime = MoveLimitTime;
        lCurrentData.OperatorTable = OperatorTable;
        //-------------------------------------------


        /* ---   Square Generator Data   --- */
        {
            lCurrentData.NumberAlongAxes = CurrentSquareGenerator->NumberAlongAxes;

            TArray<FSquareComponentData> lSquareComponentsData;
            USquareComponent* lComponent = nullptr;
            FSquareComponentData lComponentData;
            TArray<USquareComponent*> AllComponents;
            ASquare* lSquare = nullptr;

            for (int32 i = 0; i < PointerToAllSquares->Num().X; i++)
            {
                for (int32 j = 0; j < PointerToAllSquares->Num().Y; j++)
                {
                    lSquare = PointerToAllSquares->GetByIndex(i, j);

                    lSquare->GetComponents<USquareComponent>(AllComponents);

                    for (auto& Data : AllComponents)
                    {
                        lComponentData.Type = Data->GetClass();
                        lComponentData.Position = lSquare->GetData().PositionNumber;
                        lSquareComponentsData.Add(lComponentData);
                    }
                }
            }

            lCurrentData.SquareComponentsData = lSquareComponentsData;
        }
        //-------------------------------------------


        /* ---   ChessMan Generator Data | Players Data   --- */
        {
            TArray<FPlayerData> lPlayersData;
            FPlayerData lPlayerData;

            for (auto& lPlayer : *AllPlayers)
            {
                lPlayerData.Type = lPlayer->GetClass();
                lPlayerData.Position = lPlayer->GetCurrentPosition();
                lPlayersData.Add(lPlayerData);
            }

            lCurrentData.PlayersData = lPlayersData;
        }
        // PS: При расширении в несколько игроков, возможна проблема с получением информации о "мёртвых" игроках
        //-------------------------------------------


        /* ---   ChessMan Generator Data | ChessMans Data   --- */
        {
            TArray<FChessManData> lChessMansData;
            TArray<AActor*> lResultActors;

            UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AChessMan::StaticClass(), CurrentChessManGenerator->VerificationTag, lResultActors);

            for (auto& lActor : lResultActors)
            {
                lChessMansData.Add(Cast<AChessMan>(lActor)->CurrentData);
            }

            lCurrentData.ChessMansData = lChessMansData;
        }
        // PS: Массив PointerToAllChessMans не имеет информацию о "мёртвых" вражеских фигурах, только "живые",
        // поэтому в данном случае его использовать нельзя
        //-------------------------------------------


        // Предварительно
        CurrentGameInstance->SaveLevelData(lCurrentData);
    }
}

void AChessOperator::UploadLevelData()
{
    if (CurrentGameInstance)
    {
        // Текущие данные, выгружаемые из сохранения
        FLevelData lCurrentData = CurrentGameInstance->LoadLevelData();


        /* ---   Chess Operator Data   --- */

        CurrentStageNum = lCurrentData.CurrentStageNum;
        MoveLimitTime = lCurrentData.MoveLimitTime;
        OperatorTable = lCurrentData.OperatorTable;
        //-------------------------------------------


        /* ---   Square Generator Data   --- */
        {
            CurrentSquareGenerator->NumberAlongAxes = lCurrentData.NumberAlongAxes;
            CurrentSquareGenerator->RecreateBoard();

            // Массив указателей на данные
            TArray<FSquareComponentData*> lSquareComponentsData;
            for (auto& lData : lCurrentData.SquareComponentsData)
            {
                lSquareComponentsData.Add(&lData);
                /* PS: Перестройка массива на массив указателей где-то да необходим:
                * TArray<FSquareComponentData>  -- Необходимо для сохранения данных уровня
                * TArray<FSquareComponentData*> -- Результат получения данных из DataTable в ASquareGenerator
                * В теории, менее ресурсозатратный:
                * Преобразование TArray<FSquareComponentData> в TArray<FSquareComponentData*>
                */
            }

            CurrentSquareGenerator->CreateGeneratedSquareComponents(lSquareComponentsData);
        }
        //-------------------------------------------


        /* ---   ChessMan Generator Data | Players Data   --- */
        {
            // Массив указателей на данные
            TArray<FPlayerData*> lPlayersData;
            for (auto& lData : lCurrentData.PlayersData)
            {
                lPlayersData.Add(&lData);
                /* PS: Перестройка массива на массив указателей где-то да необходим:
                * Ситуация аналогична с lSquareComponentsData
                */
            }

            CurrentChessManGenerator->DeleteAllPlayers();
            CurrentChessManGenerator->CreateGeneratedPlayers(lPlayersData);
        }
        //-------------------------------------------


        /* ---   ChessMan Generator Data | ChessMans Data   --- */
        {
            // Массив указателей на данные
            TArray<FChessManData*> lChessMansData;
            for (auto& lData : lCurrentData.ChessMansData)
            {
                lChessMansData.Add(&lData);
                /* PS: Перестройка массива на массив указателей где-то да необходим:
                * Ситуация аналогична с lSquareComponentsData
                */
            }

            CurrentChessManGenerator->DeleteAllChessMans();
            CurrentChessManGenerator->CreateGeneratedChessMans(lChessMansData);
        }
        //-------------------------------------------


        /* ---   Chess Operator Data   --- */

        // Передача хода
        OnPlayersMove.Broadcast(lCurrentData.bIsPlayersMove);
        //-------------------------------------------
    }
}

void AChessOperator::SavedDataInit()
{
    CurrentGameInstance = Cast<USK_GameInstance>(GetGameInstance());

    if (!CurrentGameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s': CurrentGameInstance is NOT"),
            *GetNameSafe(this));
    }
}
//--------------------------------------------------------------------------------------



/* ---   AI   --- */

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

    // Получить массив Индексов Позиций всех "Трупов"
    PointerToAllCorpsesPositionIndex = &CurrentSquareGenerator->CorpsesPositionIndex;
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
    FIndex2D boardShape { SKUtils::GameToAI(CurrentSquareGenerator->NumberAlongAxes) };
    ChessBoardInfo->Init(boardShape.Y, boardShape.X);

    //размещаем все фигуры

    for (auto figure : *PointerToAllChessMans)
    {
        //figure почему то может быть невалидной
        if (!IsValid(figure))
        {
            UE_LOG(LogTemp, Error, TEXT("AChessOperator::PlayPrimitiveAI:: 'figure' is invalid"));
            continue;
        }

        auto chessPiece = SKUtils::ConstructChessPiece(figure->CurrentData.Type,
            PIECE_COLOR::BLACK, this);
        if (IsValid(chessPiece))
        {
            FIndex2D AICoordinatePosition { SKUtils::GameToAI(figure->CurrentData.Position) };
            ChessBoardInfo->Set(AICoordinatePosition.Y, AICoordinatePosition.X, chessPiece);
        }
    }

    //! сейчас считает только одну белую фигуру.
    // нужна либо список AChessMan* белих фигур (аналогично PointerToAllChessMans)
    // либо переменная с типом фигуры в SK_Character.h
    if (AllPlayers->Num() > 0)
    {
        auto chessPiece = SKUtils::ConstructChessPiece(EChessManType::King,
            PIECE_COLOR::WHITE, this);
        if (IsValid(chessPiece))
        {
            auto pos { SKUtils::GameToAI((*AllPlayers)[0]->GetCurrentPosition()) };
            ChessBoardInfo->Set(pos.Y, pos.X, chessPiece);
        }
    }

    //размещаем трупов
    for (auto& corps : *PointerToAllCorpsesPositionIndex)
    {
        auto pos { SKUtils::GameToAI(corps) };
        ChessBoardInfo->SetCellAccessibility(pos.Y, pos.X, false);
    }

    FOnStepCalculatedSignature OnStepCalculated;
    OnStepCalculated.BindDynamic(this, &AChessOperator::OnBlackStepCalculated);
    UChessAILibrary::GetNextStepAsync(ChessBoardInfo, PIECE_COLOR::BLACK, 5, OnStepCalculated);

}

void AChessOperator::OnBlackStepCalculated(FChessPieceStep Step)
{
    StepIsCalculatedNow = false;

    if (Step == UNABLE_MOVE)
    {
        UE_LOG(LogTemp, Error,
            TEXT("AChessOperator::OnBlackStepCalculated: no moves available or one of the teams has run out of pieces"));
        OnPlayersMove.Broadcast(true);
        return;
    }

    //поиск фигуры по индексу клетки
    FCellIndex previousPosition { SKUtils::AIToGame(Step.PreviousPosition) };
    auto figure = (*PointerToAllChessMans).FindByPredicate([previousPosition](AChessMan* m)
        {
            if (IsValid(m))
            {
                return (m->CurrentData.Position.Y == previousPosition.Y) &&
                    (m->CurrentData.Position.X == previousPosition.X);
            }
            else
            {
                UE_LOG(LogTemp, Warning,
                    TEXT("AChessOperator::OnBlackStepCalculated: 'm' is InValid"));
            }
            return false;
        });

    //фигуры почему то иногда двигаются/умирают во время рассчета хода
    if (!figure)
    {
        UE_LOG(LogTemp, Error,
            TEXT("AChessOperator::OnBlackStepCalculated: figures moved/removed while calculated"));
        OnPlayersMove.Broadcast(true);
        return;
    }


    FCellIndex newPosition { SKUtils::AIToGame(Step.NewPosition) };
    //индекс почему то иногда выходит за пределы массива PointerToAllSquares
    //(ее размер всегда равен CurrentSquareGenerator->NumberAlongAxes?)
    if (!(PointerToAllSquares->IsValidIndex(newPosition.X, newPosition.Y)))
    {
        UE_LOG(LogTemp, Error,
            TEXT("AChessOperator::OnBlackStepCalculated: invalid PointerToAllSquares index:{%d, %d}"),
            newPosition.X, newPosition.Y);
        OnPlayersMove.Broadcast(true);
        return;
    }

    //вызывается ивент, если белый король на прицеле
    if (AllPlayers->Num() > 0)
    {
        if ((*AllPlayers)[0]->GetCurrentPosition() == FIndex2D { newPosition.X, newPosition.Y })
        {
            OnKingAttacked();
        }
    }

    // Переместить выбранную фигуру на выбранную клетку
    ASquare* target = PointerToAllSquares->
        GetByIndex(FIndex2D { newPosition.X, newPosition.Y });

    (*figure)->MoveToSquare(target);
}
//--------------------------------------------------------------------------------------
