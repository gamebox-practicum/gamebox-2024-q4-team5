// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// UE:
#include "Engine/DataTable.h"

// Interaction:
#include "SK/ChessBoard/Square.h"

// Generated:
#include "SquareComponent.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// UE:
class AActor;
class UBoxComponent;
class USquareComponent;
//--------------------------------------------------------------------------------------



// Структура данных для Таблицы данных Компонентов Клеток
USTRUCT(BlueprintType)
struct FSquareComponentData : public FTableRowBase
{
    GENERATED_BODY()

    // Тип Компонента
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<USquareComponent> Type = nullptr;

    // Позиция на доске
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIntPoint Position = { 0, 0 };
    //-------------------------------------------



    /* ---   Constructors   --- */

    FSquareComponentData() {};

    FSquareComponentData(TSubclassOf<USquareComponent> iType, FIntPoint iPosition)
        : Type(iType), Position(iPosition) {};
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------



UCLASS(ClassGroup = (Custom), BlueprintType, Blueprintable, EditInlineNew,
    meta = (BlueprintSpawnableComponent))
    class SK_API USquareComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this component's properties
    USquareComponent();
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Called when the game starts
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Base   --- */

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Вызывается при создании компонента */
    virtual void OnComponentCreated() override;

    /** Вызывается при уничтожении компонента
    @param bDestroyingHierarchy - значение True, если разрушается вся иерархия компонентов.
    Позволяет избежать дорогостоящих операций
    */
    virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
    //-------------------------------------------



    /* ---   Reactions   --- */

    // Включить Реакции
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Settings|Reactions")
    bool bEnableReactions = true;
    //-------------------------------------------



    /* ---   Reactions | Events   --- */

    /** Реакция на Перекрытие коллизии */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "Reactions",
        meta = (DisplayName = "Reaction to Begin Overlap"))
    void EventReactionToBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    /** Реакция на Выход из коллизии */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "Reactions",
        meta = (DisplayName = "Reaction to End Overlap"))
    void EventReactionToEndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);
    //-------------------------------------------



    /* ---   Actor Creation   --- */

    // Создаёт выбранный Актор
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Settings|Actor Creation")
    bool bCreatesActor = false;

    // Тип стороны, занимаемая Клетку
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Settings|Actor Creation",
        meta = (EditCondition = bCreatesActor, EditConditionHides))
    EWarringPartiesType PartiesType = EWarringPartiesType::Corpse;

    // Создаваемый Актор
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Settings|Actor Creation",
        meta = (EditCondition = bCreatesActor, EditConditionHides))
    TSubclassOf<AActor> CreatedActorType = nullptr;

    // Смещение локации для создаваемого Актора
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Settings|Actor Creation",
        meta = (EditCondition = bCreatesActor, EditConditionHides))
    FVector ActorLocationOffset = FVector::ZeroVector;

    //

    /** Событие при завершении создания Актора */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "Settings|Base",
        meta = (DisplayName = "On Actor Creation"))
    void EventOnActorCreation();

    /** Получить информацию о типе текущей клетки (чёрная/белая) */
    UFUNCTION(BlueprintCallable,
        Category = "Settings|Type")
    int32 GetCurrentSquareType() const;
    //-------------------------------------------



private:

    /* ---   This Component   --- */

    // Текущая клетка-владелец
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Settings|Check",
        meta = (AllowPrivateAccess = true))
    ASquare* CurrentSquare = nullptr;

    //

    /** Инициализация данных текущего компонента*/
    void CurrentComponentInit();
    //-------------------------------------------



    /* ---   Reactions   --- */

    // Компонент реагирования (коллизия)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Settings|Check",
        meta = (EditCondition = bEnableReactions, EditConditionHides, AllowPrivateAccess = true))
    UBoxComponent* ResponseBoxComponent = nullptr;

    //

    /** Предварительная Инициализация компонентов реакции */
    void ReactionPreInit();

    /** Инициализация реакции на компонент */
    void ReactionInit();

    /** Уничтожение компонентов реакции */
    void DestroyReaction();

    /** Реакция на Перекрытие коллизии */
    UFUNCTION()
    void ReactionToBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    /** Реакция на Выход из коллизии */
    UFUNCTION()
    void ReactionToEndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);
    //-------------------------------------------



    /* ---   Actor Creation   --- */

    // Созданный Актор
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Settings|Check",
        meta = (EditCondition = bCreatesActor, EditConditionHides, AllowPrivateAccess = true))
    AActor* CreatedActor = nullptr;

    //

    /** Создать Актор над текущей Клеткой */
    void CreateActorOverSquare();

    /** Уничтожить созданный Актор */
    void DestroyCreatedActor();
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
