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



/* ---   Pre-declaration of classes and structures   --- */

// UE:
class UCapsuleComponent;

// Interaction:
class AChessManGenerator;
class AChessOperator;
class ADealerHand;
class ASquare;
class UActorMovementComponent;
class USquareComponent;
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API AChessMan : public APawn
{
    GENERATED_BODY()

public:

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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* ChessmanStaticMesh;

    // Точка местоположения Места Захвата данной фигуры Рукой Дилера
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    USceneComponent* CapturePoint = nullptr;

    // Компонент перемещения данного Актора
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UActorMovementComponent* MovementComponent;
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FChessManData CurrentData;
    //-------------------------------------------



    /* ---   Movement   --- */

    /** Переместиться к указанной Клетке */
    void MoveToSquare(ASquare* ToSquare);

    /** Установить указатель на новую Клетку */
    void SetCurrentSquare(ASquare* ToSquare);

    /** Установить указатель на текущий Оператор */
    void SetPointerToOperator(AChessOperator* CurrentOperator);

    /** Установить указатель на текущую Руку Дилера */
    void SetCurrentDealerHand(ADealerHand* CurrentDealerHand);
    //-------------------------------------------



    /* ---   Death   --- */

    // Массив Типов Компонентов Клетки, создаваемые при смерти данной Шахматной фигуры
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Death")
    TArray<TSubclassOf<USquareComponent>> SquareComponentsTypes;

    //

    /** Записать Указатель на "родительский" Генератор Шахматных фигур */
    void SetCurrentChessManGenerator(AChessManGenerator* Generator);
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
    void DealerHandMovementEnd();

    /** Событие при завершении перемещения */
    void MovementEnd();
    //-------------------------------------------



    /* ---   Death   --- */

    // Указатель на "родительский" Генератор Шахматных фигур
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    AChessManGenerator* CurrentChessManGenerator = nullptr;

    //

    /** Смерть шахматной фигуры */
    void ChessManDeath();
    //-------------------------------------------
};
