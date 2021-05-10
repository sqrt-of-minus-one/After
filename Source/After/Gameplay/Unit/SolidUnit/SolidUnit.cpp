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

#include "../../../Data/Database/Database.h"
#include "../../LogGameplay.h"
#include "../../../AfterGameModeBase.h"
#include "../../../GameConstants.h"
//#include "../../Item/Item.h"

ASolidUnit::ASolidUnit()
{
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
	AAfterGameModeBase* GameMode = GAME_MODE;
	if (!GameMode)
	{
		UE_LOG(LogGameplay, Fatal, TEXT("Auth game mode is not AAfterGameModeBase"));
	}

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	SolidUnitData = &Database->GetSolidUnitData(Id);

	CollisionComponent->SetBoxExtent(GameConstants::TileSize * FVector(SolidUnitData->Size, 1.f));
	if (DamageBoxComponent) // Damage box can be destroyed by AUnit::BeginPlay
	{
		DamageBoxComponent->SetBoxExtent(GameConstants::TileSize * FVector(SolidUnitData->Size, 1.f) + GameConstants::DamageBoxDelta);
	}
	if (SeemsDangerousBoxComponent) // Can be destroyed by AUnit::BeginPlay
	{
		SeemsDangerousBoxComponent->SetBoxExtent(GameConstants::TileSize * FVector(SolidUnitData->Size, 1.f) + FVector(UnitData->SeemsDangerousDelta, UnitData->SeemsDangerousDelta, GameConstants::TileSize.Z));
	}
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
		MeshComponent = SpriteComponent;

		SelectionSpriteComponent->AttachToComponent(SpriteComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		BreakSpriteComponent->AttachToComponent(SpriteComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		FlipbookComponent->DestroyComponent();
		FlipbookComponent = nullptr;

		SpriteComponent->SetSprite(SolidUnitData->Sprite);
	}

	if (UnitData->bSelectable)
	{
		if (!Database->GetExtraData().SelectionSprites.Contains(SolidUnitData->Size) ||
			!Database->GetExtraData().SelectionSprites[SolidUnitData->Size])
		{
			UE_LOG(LogGameplay, Error, TEXT("Database doesn't contain selection sprite with size %dx%d"), SolidUnitData->Size.X, SolidUnitData->Size.Y);
		}
		else
		{
			SelectionSpriteComponent->SetSprite(Database->GetExtraData().SelectionSprites[SolidUnitData->Size]);
		}
	}

	Health = SolidUnitData->MaxHealth;
}

void ASolidUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FSolidUnitInfo& ASolidUnit::GetSolidUnitData() const
{
	return *SolidUnitData;
}

float ASolidUnit::GetHealth() const
{
	return Health;
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
	GetWorld()->DestroyActor(this);
}

//////////////////////////////////////////////////////////////////////
//                          o 08O                                   //
//                       o8O0 o                  \|/                //
//                      O0 Oo0O                  -0-                //
//               _      o 80                     /|\                //
//              / \     0O                                          //
//             /   \    o                                           //
//            /     \  ||              01001                        //
//           /       \ ||            000011001                      //
//          /   ___   \||           01011011000                     //
//         /   |_|_|   \|           11011000110                     //
//        /    |_|_|    \           11110010000                     //
//       /_______________\          10010000000                     //
//       |               |           111010001                      //
//       |    WELCOME    |             01001      It's the          //
//       |      ___      |              | |      binary tree!       //
//       |     |   |     |              | |                         //
//       |     |  _|     |              | |    It says to you,      //
//       |     |   |     |              / \           Hello! :)     //
//       |     |   |     |             /| |\                        //
///////////////////////////////////////ROOT!//////////////////////////

void ASolidUnit::Break(/* const UItem* By */)
{
	// Todo: Drop
	GetWorld()->DestroyActor(this);
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
