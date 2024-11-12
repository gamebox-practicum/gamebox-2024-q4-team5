// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Tools:
#include "SK/Tools/Index2D.h"

// Generated:
#include "SK_Character.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// UE:
class UCameraComponent;

// Interaction:
class ASquare;
class AChessOperator;
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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* ChessmanSkeletalMesh;

    // Статичный Меш Шахматной Фигуры
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* ChessmanStaticMesh;
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
    //-------------------------------------------



    /* ---   Input Action   --- */

    // Базовая скорость поворота, град/сек
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Camera")
    float BaseTurnRate = 1.f;

    // Базовая скорость обзора вверх/вниз в градусах в секунду
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Camera")
    float BaseLookUpRate = 1.f;
    //-------------------------------------------



    /* ---   Movement   --- */

    // Скорость перемещения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
    float MovementSpeed = 10.0f;

    // Максимальное отклонение.
    // Если местоположение меньше данного отклонения, то телепортируем персонажа
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
    float MaxDeviation = 0.1f;

    //

    // Переместиться к указанной клетке
    void MoveToSquare(ASquare* ToSquare);

    // Получить текущую ползицию на Шахматной доске
    FIndex2D GetCurrentPosition();
    //-------------------------------------------



    /* ---   Player Moves Sequence   --- */

    UFUNCTION()
    void PlayerMovesSequence(bool bIsPlayersMove);
    //-------------------------------------------



private:

    /* ---   Base   --- */

    /* Очистка от неиспользуемых компонентов */
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

    // Указатель на текущую ячейку (необходимо исключения передвижения на ту же клетку)
    ASquare* CurrentSquare = nullptr;

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
    void EnableMouse(bool bEnabled);
    //-------------------------------------------



    /* ---   Player Moves Sequence   --- */

    // Указатель на текущего Оператора
    AChessOperator* CurrentChessOperator = nullptr;

    //

    /**	Подписаться на делегаты */
    void SubscribeToDelegates();
    //-------------------------------------------
};
