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
class ASquare;
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

    // Переместиться к указанной клетке
    void MoveToSquare(ASquare* ToSquare);

    // Установить указатель на новую ячейку
    void SetCurrentSquare(ASquare* ToSquare);
    //-------------------------------------------



private:

    /* ---   Base   --- */

    /* Очистка от неиспользуемых компонентов */
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

    // Расчёт передвижения на каждый кадр
    void MovementForTick(const float& DeltaTime);
    //-------------------------------------------
};
