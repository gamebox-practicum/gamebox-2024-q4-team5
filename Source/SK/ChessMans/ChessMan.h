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

// Interaction:
class AChessManGenerator;
class ASquare;
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

    // Скорость перемещения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
    float MovementSpeed = 10.0f;

    // Максимальное отклонение.
    // Если местоположение меньше данного отклонения, то телепортируем персонажа
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Movement")
    float MaxDeviation = 0.1f;

    //

    /** Переместиться к указанной Клетке */
    void MoveToSquare(ASquare* ToSquare);

    /** Установить указатель на новую Клетку */
    void SetCurrentSquare(ASquare* ToSquare);
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

    // Флаг контроля перемещения
    bool bIsMovingToNewLocation = false;

    // Новая локация к которой движемся
    FVector NewLocation;

    //

    /** Расчёт передвижения на каждый кадр */
    void MovementForTick(const float& DeltaTime);
    //-------------------------------------------


    /* ---   Death   --- */

    // Указатель на "родительский" Генератор Шахматных фигур
    AChessManGenerator* CurrentChessManGenerator = nullptr;

    //

    /** Смерть шахматной фигуры */
    void ChessManDeath();
    //-------------------------------------------
};
