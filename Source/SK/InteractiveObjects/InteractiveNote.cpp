//

// Base:
#include "InteractiveNote.h"

// Interaction:
#include "SK/Core/SK_GameInstance.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values
AInteractiveNote::AInteractiveNote()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Preliminarily



    /* ---   Components   --- */

    // Корневой компонент
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Главный Меш
    NoteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Block Mesh"));
    NoteMesh->SetupAttachment(RootComponent);
    //NoteMesh->SetRelativeLocation(FVector(0.f, 0.f, -25.f));
    //NoteMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.5f));
    NoteMesh->SetCastShadow(false);
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

// Called when the game starts or when spawned
void AInteractiveNote::BeginPlay()
{
    Super::BeginPlay();

    InitNoteData();
}
//--------------------------------------------------------------------------------------



/* ---   Interaction   --- */

void AInteractiveNote::NotifyActorBeginCursorOver()
{
    Super::NotifyActorBeginCursorOver();

    // Контроль выделения (свечения)
    NoteMesh->SetRenderCustomDepth(true);
}

void AInteractiveNote::NotifyActorEndCursorOver()
{
    Super::NotifyActorEndCursorOver();

    // Контроль выделения (свечения)
    NoteMesh->SetRenderCustomDepth(false);
}

void AInteractiveNote::NotifyActorOnClicked(FKey ButtonReleased)
{
    Super::NotifyActorOnClicked(ButtonReleased);

    if (ButtonReleased == EKeys::LeftMouseButton)
    {
        if (CurrentGameInstance)
        {
            CurrentGameInstance->TakingNote(NoteType);

            Destroy();
        }
    }
}
//--------------------------------------------------------------------------------------



/* ---   Note System   --- */

void AInteractiveNote::InitNoteData()
{
    CurrentGameInstance = Cast<USK_GameInstance>(GetGameInstance());

    if (CurrentGameInstance)
    {
        if (CurrentGameInstance->CheckNote(NoteType))
        {
            // Если данная Записка была собрана, то убрать её из-за ненадобности
            Destroy();
            // PS: Следует изменить условие создания Записки,
            // чтобы она в принципе не появлялась и не инициализировалась
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s': CurrentGameInstance is NOT"),
            *GetNameSafe(this));
    }
}

TArray<FName> AInteractiveNote::GetAuthorOptions() const
{
    if (NotesTable)
    {
        return NotesTable->GetRowNames();
    }

    return TArray<FName>();
}
//--------------------------------------------------------------------------------------
