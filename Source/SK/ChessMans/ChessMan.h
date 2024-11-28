// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// Structs:
#include "ChessManStruct.h"

// Generated:
#include "ChessMan.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Delegates   --- */

// Делегат: При Смерти фигуры
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

// Делегат: При "Поедании" Шахматной фигуры
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEatingChessman);
// ----------------------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes and structures   --- */

// UE:
class UCapsuleComponent;

// Interaction:
class AChessManGenerator;
class AChessOperator;
class ADealerHand;
class ACharacter;
class ASK_Character;
class ASquare;
class USquareComponent;

// Tools:
class UActorMovementComponent;
class UActorRotationComponent;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API AChessMan : public APawn
{
    GENERATED_BODY()

public:

    /* ---   Delegates   --- */

    // Делегат: При Смерти фигуры
    UPROPERTY(BlueprintAssignable)
    FOnDeath OnDeath;

    // Делегат: При "Поедании" Шахматной фигуры
    UPROPERTY(BlueprintAssignable)
    FOnEatingChessman OnEatingChessman;
    //-------------------------------------------



    /* ---   Constructors   --- */

    // Sets default values for this pawn's properties
    AChessMan();
    //-------------------------------------------



    /* ---   Components   --- */

    // Капсула коллизии
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UCapsuleComponent* CapsuleComponent;

    // Меш Шахматной Фигуры со скелетом
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* ChessmanSkeletalMesh;

    // Статичный Меш Шахматной Фигуры
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
    //    meta = (AllowPrivateAccess = "true"))
    //UStaticMeshComponent* ChessmanStaticMesh;

    // Точка местоположения Места Захвата данной фигуры Рукой Дилера
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    USceneComponent* CapturePoint = nullptr;

    // Компонент перемещения данного Актора
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UActorMovementComponent* MovementComponent;

    // Компонент вращения данного Актора
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UActorRotationComponent* RotationComponent;
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Base   --- */

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void Destroyed() override;

    /** Инициализация всех данных */
    void Initialize();

    /** Событие при завершении инициализации */
    UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Base",
        meta = (DisplayName = "On Initialize Complete"))
    void EventOnInitializeComplete();
    //-------------------------------------------



    /* ---   Interaction   --- */

    /** Событие, когда на него наводят курсор мыши с помощью интерактивного интерфейса */
    virtual void NotifyActorBeginCursorOver() override;

    /** Событие, когда с него убирают курсор мыши с помощью интерактивного интерфейса */
    virtual void NotifyActorEndCursorOver() override;

    /** Событие, когда этот субъект находится под мышкой при нажатии клавиши реагирования с помощью интерактивного интерфейса */
    virtual void NotifyActorOnClicked(FKey ButtonReleased) override;
    //-------------------------------------------



    /* ---   Type   --- */

    // Текущая информация о данной Фигуре
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Type")
    FChessManData CurrentData;

    //

    /** Получить информацию о типе текущей клетки (чёрная/белая) */
    UFUNCTION(BlueprintCallable, Category = "Settings|Type")
    int32 GetCurrentSquareType() const;
    //-------------------------------------------



    /* ---   Movement   --- */

    // Игнорировать тип Клетки
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
    bool bOnlyToUp = false;

    // Высота подъёма при перемещении
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
    float LiftingHeight = 200.f;

    // Расстояние смены варианта хода на вариант хода сверху
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
    FIndex2D DistanceToUp = FIndex2D(2);

    //

    /** Переместиться к указанной Клетке */
    void MoveToSquare(ASquare* ToSquare);

    /** Установить указатель на новую Клетку */
    void SetCurrentSquare(ASquare* NewSquare);

    /** Установить указатель на текущий Оператор */
    void SetPointerToOperator(AChessOperator* CurrentOperator);

    /** Установить указатель на текущую Руку Дилера */
    void SetCurrentDealerHand(ADealerHand* CurrentDealerHand);

    /** Проверка и изменение типа перемещения */
    void CheckMovementType(ASquare* NewSquare);

    /** Преобразование пешки в другую рандомную фигуру */
    bool PawnTransformation();
    //-------------------------------------------



    /* ---   Rotation   --- */

    // Тип поворота данной Шахматной фигуры
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Rotation")
    EChessManRotationType RotationType = EChessManRotationType::NONE;
    //-------------------------------------------



    /* ---   Death   --- */

    // Массив Типов Компонентов Клетки, создаваемые при смерти данной Шахматной фигуры
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Death")
    TArray<TSubclassOf<USquareComponent>> SquareComponentsTypes;

    //

    /** Записать Указатель на "родительский" Генератор Шахматных фигур */
    void SetCurrentChessManGenerator(AChessManGenerator* Generator);
    //-------------------------------------------



    /* ---   Sounds   --- */

    // Флаг, сигнализирующий о "раздавливании" на белую фигуру
    bool bIsFigureBeingCrushed = false;

    //

    /** Событие при завершении хода сверху (шага) */
    UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Sounds",
        meta = (DisplayName = "On Stepping from Above"))
    void EventOnSteppingFromAbove(bool IsFigureBeingCrushed);
    //-------------------------------------------



private:

    /* ---   Base   --- */

    /** Очистка от неиспользуемых компонентов */
    void Cleaning();
    //-------------------------------------------



    /* ---   Movement   --- */

    // Указатель на текущую ячейку (необходимо контроля передвижения)
    UPROPERTY()
    ASquare* CurrentSquare = nullptr;

    // Флаг контроля при перемещении Актора
    bool bIsMovingToNewLocation = false;

    // Флаг типа перемещения
    bool bMovementTypeToUp = false;

    // Указатель на текущего Оператора
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    AChessOperator* CurrentOperator = nullptr;

    // Указатель на текущую Руку Дилера
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    ADealerHand* CurrentDealerHand = nullptr;

    //

    /** Событие при завершении перемещения Руки Дилера */
    UFUNCTION()
    void DealerHandMovementEnd();

    /** Событие при завершении перемещения */
    UFUNCTION()
    void MovementEnd();

    /** Событие при завершении перемещения вверх */
    UFUNCTION()
    void MovementEnd_Up();

    /** Запуск перемещения на клетку */
    UFUNCTION()
    void MovementEnd_ToSquare();
    //-------------------------------------------



    /* ---   Rotation   --- */

    // Указатель на игрока, в сторону которого происходит поворот
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    ASK_Character* CurrentFirstPlayer = nullptr;

    //

    /**	Подписаться на делегаты */
    void SubscribeToDelegates();

    /** Инициализация данных для Ротации */
    void RotationInit();

    /** Поворот в сторону игрока с учётом выбранного типа поворота */
    void RotateToFirstPlayer(const bool& bIsPlayersMove = false);

    /** Получение ближайшей к игроку локации */
    FVector GetClosestToPlayer(const TArray<FIndex2D>& Variants);
    //-------------------------------------------



    /* ---   Death   --- */

    // Указатель на "родительский" Генератор Шахматных фигур
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    AChessManGenerator* CurrentChessManGenerator = nullptr;

    // Флаг контроля действий при смерти данной Шахматной фигуры
    bool bIsDead = false;

    //

    /** Смерть шахматной фигуры */
    void ChessManDeath();

    /** Отписаться от делегатов */
    void UnsubscribeToDelegates();
    //-------------------------------------------
};
