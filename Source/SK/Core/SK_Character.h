//

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Structs:
#include "SK/ChessMans/ChessManStruct.h"

// Generated:
#include "SK_Character.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// UE:
class UCameraComponent;

// Interaction:
class AChessOperator;
class AChessManGenerator;
class ADealerHand;
class ASquare;
class ASK_PlayerController;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API ASK_Character : public ACharacter
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Устанавливает значения по умолчанию для свойств этого персонажа
    ASK_Character();
    //-------------------------------------------



    /* ---   Components   --- */

    // Камера от первого лица
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FirstPersonCameraComponent;

    // Меш Шахматной Фигуры со скелетом
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
    //    meta = (AllowPrivateAccess = "true"))
    //USkeletalMeshComponent* ChessmanSkeletalMesh;

    // Статичный Меш Шахматной Фигуры
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
    //    meta = (AllowPrivateAccess = "true"))
    //UStaticMeshComponent* ChessmanStaticMesh;
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Вызывается при запуске игры или при запуске spawn
    virtual void BeginPlay() override;
    //-------------------------------------------



    /* ---   Input Action   --- */

    /** Вызывается для привязки функциональности к входным данным */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    //-------------------------------------------



public:

    /* ---   Base   --- */

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /** Вызывается, когда эта пешка захвачена */
    virtual void PossessedBy(AController* NewController) override;

    /**	Событие, когда этот субъект перекрывается с другим */
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

    /** Инициализация данных */
    void Initialize();
    //-------------------------------------------



    /* ---   Input Action   --- */

    // Базовая скорость поворота, град/сек
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Camera")
    float BaseTurnRate = 1.f;

    // Базовая скорость обзора вверх/вниз в градусах в секунду
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Camera")
    float BaseLookUpRate = 1.f;

    //

    /** Событие при атаке (нанесении удара врагу) */
    UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Input Action",
        meta = (DisplayName = "On Attack"))
    void EventOnAttack();
    //-------------------------------------------



    /* ---   Movement   --- */

    // Указатель на текущую ячейку (необходимо исключения передвижения на ту же клетку)
    ASquare* CurrentSquare = nullptr;

    // Флаг блокировки управления игрока
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
    bool bPlayerControlLock = false;

    // Позиция на доске
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIntPoint CurrentPosition;

    // Скорость перемещения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
    float MovementSpeed = 10.0f;

    // Максимальное отклонение.
    // Если местоположение меньше данного отклонения, то телепортируем персонажа
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
    float MaxDeviation = 0.1f;

    //

    /** Переместиться к указанной клетке */
    void MoveToSquare(ASquare* ToSquare);

    /** Событие при передвижении персонажа */
    UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Movement",
        meta = (DisplayName = "On Move"))
    void EventOnMove();

    /** Получить текущую позицию на Шахматной доске */
    FIntPoint GetCurrentPosition() const;
    //-------------------------------------------



    /* ---   Player Moves Sequence   --- */

    /** Контроль передачи хода игроку */
    UFUNCTION()
    void PlayerMovesSequence(const bool& bIsPlayersMove);

    /** Установить указатель на текущий Оператор */
    void SetPointerToOperator(AChessOperator* CurrentOperator);

    /** Записать Указатель на "родительский" Генератор Шахматных фигур */
    void SetCurrentChessManGenerator(AChessManGenerator* Generator);
    //-------------------------------------------



    /* ---   Rotation   --- */

    /** Поворот в сторону игрока с учётом выбранного типа поворота */
    void RotateToActor(AActor* Actor);
    //-------------------------------------------



    /* ---   Death   --- */

    /** Установить указатель на текущую Руку Дилера */
    void SetCurrentDealerHand(ADealerHand* CurrentDealerHand);

    /** Запуск смерти от Руки Дилера */
    void DeathByDealerHand();

    /** Запустить метод смерти ГГ */
    UFUNCTION()
    void CharacterDeath();

    /** Событие при смерти персонажа */
    UFUNCTION(BlueprintImplementableEvent, Category = "Settings|Death",
        meta = (DisplayName = "On Death"))
    void EventOnDeath();
    //-------------------------------------------



private:

    /* ---   Base   --- */

    /** Очистка от неиспользуемых компонентов */
    void Cleaning();
    //-------------------------------------------



    /* ---   Input Action   --- */

    // Текущий контроллер игрока
    ASK_PlayerController* CurrentPlayerController;

    //

    /** Перемещение вперёд/назад */
    void MoveForward(float Val);

    /** Перемещение влево/вправо */
    void MoveRight(float Val);

    /**	Поворот с заданной скоростью */
    void TurnAtRate(float Rate);

    /**	Обзор вверх/вниз с заданной скоростью */
    void LookUpAtRate(float Rate);
    //-------------------------------------------



    /* ---   Movement   --- */

    // Новая локация к которой движемся
    FVector NewLocation;

    // Флаг контроля перемещения
    bool bIsMovingToNewLocation = false;

    // Флаг разрешения хода Игрока
    bool bIsMoveAllowed = true;

    //

    /**	Расчёт передвижения на каждый кадр */
    void MovementForTick(const float& DeltaTime);

    /**	Управление мышью */
    void EnableMouse(const bool& bEnabled);
    //-------------------------------------------



    /* ---   Player Moves Sequence   --- */

    // Указатель на текущего Оператора
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    AChessOperator* CurrentOperator = nullptr;

    // Указатель на "родительский" Генератор Шахматных фигур
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    AChessManGenerator* CurrentChessManGenerator = nullptr;

    //

    /**	Подписаться на делегаты */
    void SubscribeToDelegates();
    //-------------------------------------------



    /* ---   Rotation   --- */

    // Шахматная фигура, на которую поворачиваем камеру
    AActor* ActorForRotation = nullptr;

    //

    /** Поворот в сторону игрока с учётом выбранного типа поворота */
    void RotateToActorForTick(const float& lDeltaTime);
    //-------------------------------------------



    /* ---   Death   --- */

    // Указатель на текущую Руку Дилера
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    ADealerHand* CurrentDealerHand = nullptr;
    //-------------------------------------------
};
