//

// Base:
#include "ChessGenerator.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values
AChessGenerator::AChessGenerator()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Предварительно

}
//--------------------------------------------------------------------------------------



/* ---   Tools   --- */

//// Could not reassign to cpp: LNK2019
//
//template <class T, class>
//inline TArray<T*> AChessGenerator::GetAllActors(const FName& iTag)
//{
//    TArray<T*> lResult;
//    TArray<AActor*> lResultActors;
//
//    UGameplayStatics::GetAllActorsOfClass(GetWorld(), T::StaticClass(), lResultActors);
//
//    if (lResultActors.IsValidIndex(0))
//    {
//        for (AActor* lData : lResultActors)
//        {
//            T* lCurrentChessMan = Cast<T>(lData);
//
//            if (iTag == "")
//            {
//                lResult.Add(lCurrentChessMan);
//            }
//            else
//            {
//                for (FName lTags : lCurrentChessMan->Tags)
//                {
//                    if (lTags == iTag)
//                    {
//                        lResult.Add(lCurrentChessMan);
//                        break;
//                    }
//                }
//            }
//        }
//    }
//
//    return lResult;
//}
//--------------------------------------------------------------------------------------
