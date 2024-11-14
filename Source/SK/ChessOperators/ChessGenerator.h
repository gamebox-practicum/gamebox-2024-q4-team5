// 

#pragma once

// Base:
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// UE:
#include "Kismet/GameplayStatics.h"

// Generated:
#include "ChessGenerator.generated.h"
//--------------------------------------------------------------------------------------



UCLASS()
class SK_API AChessGenerator : public AActor
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    AChessGenerator();
    //-------------------------------------------



public:

    /* ---   Tools   --- */

    /** Получить все Элементы (Актор или его наследники) с конкретным Тегом

    @param  Tag -- Тег фильтрации. По умолчанию пуст (равен "")

    @return Массив указателей указанного типа (Актор или его наследники)
    Если требуется получить без учёта Тега, то Tag должен быть пустым (равным "")
    */
    template<class T, class = std::enable_if<std::is_base_of<AActor, T>::value>::type>
    TArray<T*> GetAllActors(const FName& Tag = "")
    {
        TArray<T*> lResult;
        TArray<AActor*> lResultActors;

        UGameplayStatics::GetAllActorsOfClass(GetWorld(), T::StaticClass(), lResultActors);

        if (lResultActors.IsValidIndex(0))
        {
            for (AActor* lData : lResultActors)
            {
                T* lCurrentChessMan = Cast<T>(lData);

                if (Tag == "")
                {
                    lResult.Add(lCurrentChessMan);
                }
                else
                {
                    for (FName lTags : lCurrentChessMan->Tags)
                    {
                        if (lTags == Tag)
                        {
                            lResult.Add(lCurrentChessMan);
                            break;
                        }
                    }
                }
            }
        }

        return lResult;
    }
    //-------------------------------------------
};
