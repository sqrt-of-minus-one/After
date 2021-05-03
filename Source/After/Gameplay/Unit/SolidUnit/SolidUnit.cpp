    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: SolidUnit.cpp               //
////////////////////////////////////////

#include "SolidUnit.h"

#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/AudioComponent.h"

#include "../../../AfterGameModeBase.h"
//#include "../../Item/Item.h"

ASolidUnit::ASolidUnit()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent->SetCollisionProfileName(FName("SolidUnit"));

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	SpriteComponent->SetupAttachment(GetRootComponent());

	BreakSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Break Sprite"));
	BreakSpriteComponent->SetupAttachment(FlipbookComponent);
	BreakSpriteComponent->SetVisibility(false);
}

void ASolidUnit::BeginPlay()
{
	Super::BeginPlay();

	// Get game mode
	AAfterGameModeBase* GameMode = Cast<AAfterGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Auth game mode is not AAfterGameModeBase"));
	}

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	SolidUnitData = &Database->GetSolidUnitData(Id);

	CollisionComponent->SetBoxExtent(AAfterGameModeBase::TileSize * FVector(SolidUnitData->Size, 1.f));
	SpriteComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	SpriteComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	BreakSpriteComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	if (SolidUnitData->bUseFlipbook)
	{
		MeshComponent = FlipbookComponent;
		SpriteComponent->DestroyComponent();
		SpriteComponent = nullptr;

		FlipbookComponent->SetFlipbook(SolidUnitData->Flipbook);
	}
	else
	{
		SelectionSpriteComponent->SetupAttachment(SpriteComponent);
		BreakSpriteComponent->SetupAttachment(SpriteComponent);
		FlipbookComponent->DestroyComponent();
		FlipbookComponent = nullptr;

		SpriteComponent->SetSprite(SolidUnitData->Sprite);
	}

	if (UnitData->bSelectable &&
		(!Database->GetExtraData().SelectionSprites.Contains(SolidUnitData->Size) ||
			!Database->GetExtraData().SelectionSprites[SolidUnitData->Size]))
	{
		UE_LOG(LogTemp, Error, TEXT("Database doesn't contain selection sprite with size %dx%d"), SolidUnitData->Size.X, SolidUnitData->Size.Y);
	}
	else
	{
		SelectionSpriteComponent->SetSprite(Database->GetExtraData().SelectionSprites[SolidUnitData->Size]);
	}

	Health = SolidUnitData->MaxHealth;
}

void ASolidUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASolidUnit::Interact()
{
	// This should be pure virtual, but it's Unreal...
}

void ASolidUnit::Damage(float Value, FDamageType Type, const AActor* FromWho)
{
	Health -= Value * SolidUnitData->DamageResist[Type];

	if (Health <= 0.f)
	{
		Health = 0.f;
		Kill(Type, FromWho);
	}
}

void ASolidUnit::StartBreaking(/* const UItem* By */)
{
	// Todo
}

void ASolidUnit::StopBreaking()
{
	// Todo
}

void ASolidUnit::Kill(FDamageType Type, const AActor* Murderer)
{
	Destroy();
}

void ASolidUnit::Break(/* const UItem* By */)
{
	// Todo: Drop
	Destroy();
}

const FSolidUnitInfo& ASolidUnit::GetSolidUnitData() const
{
	return *SolidUnitData;
}

void ASolidUnit::PlaySound(FSolidUnitSoundType Sound)
{
	int Size = SolidUnitData->Sounds.Sounds[Sound].Sounds.Num();
	if (Size != 0)
	{
		AudioComponent->SetSound(SolidUnitData->Sounds.Sounds[Sound].Sounds[FMath::RandRange(0, Size - 1)]);
		AudioComponent->Play();
	}
}
