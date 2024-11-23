//

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Tools:
#include "SK/Tools/Index2D.h"

// Structs:
#include "SK/ChessBoard/SquareStruct.h"
#include "SK/Core/SK_GameInstance.h"
#include "SK/Tools/Chess_AI/ChessBoardInfo.h"
#include "ChessOperatorStruct.h"

// Generated:
#include "ChessOperator.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Delegates   --- */

// Делегат хода Игроков
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayersMove, const bool&);
// ----------------------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// UE:
class UDataTable;

// Interaction:
class ASquareGenerator;
class AChessManGenerator;
class AChessMan;
class ASK_Character;
class ATimeBeaconGenerator;
class ATimeBeacon;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API AChessOperator : public AActor
{
    GENERATED_BODY()

public:

    /* ---   Delegates   --- */

    FOnPlayersMove OnPlayersMove; // Делегат хода Игроков
    // ----------------------------------------------------------------------------------------------------



    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    AChessOperator();
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Base   --- */

    /** Вызывается при размещении экземпляра данного класса (в редакторе) или его запуске.
    @param	Transform	-- Трансформация данного Актора.
    */
    virtual void OnConstruction(const FTransform& Transform) override;
    //-------------------------------------------



    /* ---   Re Generate   --- */

    /** Перегенерировать (перезапустить) данный Генератор */
    UFUNCTION(BlueprintCallable, Category = "Settings", CallInEditor)
    void ReGenerate();
    //-------------------------------------------



    /* ---   Generators | Square Generator   --- */

    // Указатель на текущий Генератор Клеток
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generators|Square Generator")
    ASquareGenerator* CurrentSquareGenerator;

    // Количество клеток доски вдоль осей
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generators|Square Generator")
    FIndex2D NumberAlongAxes = { 10, 10 };
    //-------------------------------------------



    /* ---   Generators | Time Beacon Generator   --- */

    // Указатель на текущий Генератор Шахматных фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generators|Time Beacon Generator")
    ATimeBeaconGenerator* CurrentTimeBeaconGenerator;

    // Тип Маяка времени (Колонны)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Generators|Time Beacon Generator")
    TSubclassOf<ATimeBeacon> BeaconType;
    //-------------------------------------------



    /* ---   Generators | ChessMan Generator   --- */

    // Указатель на текущий Генератор Шахматных фигур
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generators|ChessMan Generator")
    AChessManGenerator* CurrentChessManGenerator;

    //

    /** Обновить данные текущего Генератора Шахматных фигур */
    void UpdateCurrentTimeBeaconGenerator(const FVector& BlockSize);
    //-------------------------------------------



    /* ---   Player Moves Sequence   --- */

    // Время таймера в секундах
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Moves Sequence")
    float MoveLimitTime = 20.f;

    //

    /** Останов таймера: Последовательность ходов */
    void StopTimer_MovesSequence();
    //-------------------------------------------



    /* ---   Generators   --- */

    // Таблица данных конструирования уровня
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Generators",
        meta = (RequiredAssetDataTags = "RowStructure=ChessOperatorData"))
    UDataTable* OperatorTable;
    //-------------------------------------------



    /* ---   Stage   --- */

    /** Переключиться на следующий этап */
    void ToNextStage();
    //-------------------------------------------



    /* ---   Attack   --- */

    // Воспроизводимый звук при Атаке
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Attack")
    USoundBase* WalkingSound = nullptr;
    //-------------------------------------------



    /* ---   Level Saving   --- */

    /** Сохранить данные уровня */
    UFUNCTION(BlueprintCallable, Category = "Level Saving")
    void SaveLevelData() const;

    /** Выгрузить сохранённых данных уровня */
    //UFUNCTION(BlueprintCallable, Category = "Level Saving")
    void UploadLevelData();
    //-------------------------------------------



private:

    /* ---   Delegate   --- */

    // Флаг управления ходами Оператора
    bool bSkipOperatorTurn = false;

    //

    /** Функция делегата: Смена хода */
    UFUNCTION()
    void PlayerMovesSequence(const bool& bIsPlayersMove);
    //-------------------------------------------



    /* ---   Generators   --- */

    // Массив данных конструирования уровня, полученный из Таблицы данных "OperatorTable"
    TArray<FChessOperatorData*> CurrentOperatorData;

    // Номер текущего этапа
    int32 CurrentStageNum = 0;

    //

    /** Template: Получить первый Актор конкретного типа */
    template<typename T>
    T* GetFirstActor();

    /** Предварительная Инициализация из Таблицы данных Оператора */
    void OperatorDataPreInit();

    /** Получить Абсолютное (полное) количество размера доски */
    FIndex2D GetFullNumberAlongAxes();
    //-------------------------------------------



    /* ---   Generators | Square Generator   --- */

    /** Проверка указателя на текущий Генератор Клеток */
    void UpdateCurrentSquareGenerator();

    /** Проверка и получение указателя на текущий Генератор Клеток */
    ASquareGenerator* GetCurrentSquareGenerator();

    /** Получить первый (по порядку на карте) Генератор Клеток */
    ASquareGenerator* GetFirstSquareGenerator();
    //-------------------------------------------



    /* ---   Generators | ChessMan Generator   --- */

    /** Обновить данные текущего Генератора Шахматных фигур */
    void UpdateCurrentChessManGenerator();

    /** Проверка и получение указателя на текущий Генератор Шахматных фигур */
    AChessManGenerator* GetCurrentChessManGenerator();

    /** Получить первый (по порядку на карте) Генератор Шахматных фигур */
    AChessManGenerator* GetFirstChessManGenerator();
    //-------------------------------------------



    /* ---   Generators | Time Beacon Generator   --- */

    /** Проверка и получение указателя на текущий Генератор Шахматных фигур */
    ATimeBeaconGenerator* GetCurrentTimeBeaconGenerator();

    /** Получить первый (по порядку на карте) Генератор Шахматных фигур */
    ATimeBeaconGenerator* GetFirstTimeBeaconGenerator();
    //-------------------------------------------



    /* ---   Attack   --- */

    /** Запуск звука атаки */
    void PlayingAttackSound();
    //-------------------------------------------



    /* ---   Primitive AI   --- */

    // Массив из всех Шахматных фигур
    TArray<AChessMan*>* PointerToAllChessMans = nullptr;

    // Массив из Шахматных фигур, доступных для хода
    TArray<AChessMan*>* PointerToAllAvailableChessMans = nullptr;

    // Массив из всех игроков
    TArray<ASK_Character*>* AllPlayers = nullptr;

    // Двумерный массив указателей на Клетки
    FSquareArray2D* PointerToAllSquares = nullptr;

    // Массив Индексов Позиций всех "Трупов"
    TArray<FIndex2D>* PointerToAllCorpsesPositionIndex = nullptr;

    //

    /** Инициализация данных для Примитивного ИИ */
    void PrimitiveAI_Init();

    /** Запустить ход Примитивного ИИ */
    void PlayPrimitiveAI();
    //-------------------------------------------



    /* ---   Player Moves Sequence   --- */

    // Таймер для режима Мячей
    FTimerHandle Timer_MovesSequence;

    //

    /** Инициализация таймера: Последовательность ходов */
    void TimerInit_MovesSequence();

    /** Реакция таймера: Передача хода Оператору */
    void TimerAction_OperatorMove() const;

    /** Реакция таймера: Передача хода Игроку */
    void TimerAction_PlayersMove() const;
    //-------------------------------------------



    /* ---   Level Saving   --- */

    // Текущий GameInstance
    USK_GameInstance* CurrentGameInstance = nullptr;

    //

    /** Предварительная инициализация данных для сохранения */
    void SavedDataInit();
    //-------------------------------------------



    // юпроперти предотвращающая удаление даныых, использующихся в другом потоке, гк
    UPROPERTY()
    UChessBoardInfo* ChessBoardInfo;

    UFUNCTION()
    void OnBlackStepCalculated(FChessPieceStep Step);

    //для запрета вызова рассчета хода если не рассчитался предыдущий
    bool StepIsCalculatedNow = false;
};
