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
#include "../../Item/ThrownItem.h"
#include "../../Item/Item.h"

ASolidUnit::ASolidUnit() :
	Breaking(0.f),
	BreakingStage(-1)
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent->SetCollisionProfileName(FName("SolidUnit"));

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	SpriteComponent->SetupAttachment(GetRootComponent());
	SpriteComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));

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
	BreakProfileData = &Database->GetBreakProfileData(SolidUnitData->BreakProfile);

	CollisionComponent->SetBoxExtent(GameConstants::TileSize * FVector(SolidUnitData->Size, 1.f));
	if (DamageBoxComponent) // Damage box can be destroyed by AUnit::BeginPlay
	{
		DamageBoxComponent->SetBoxExtent(GameConstants::TileSize * FVector(SolidUnitData->Size, 1.f) + GameConstants::DamageBoxDelta);
	}

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
		SelectionSpriteComponent->SetWorldLocation(GetActorLocation());
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

	if (Destroyers.Num() != 0)
	{
		for (TPair<int, FDestroyerInfo>& i : Destroyers)
		{
			if (!i.Value.Item || !i.Value.Item->IsPendingKill())
			{
				Breaking += DeltaTime * i.Value.SpeedMultiplier;
				if (i.Value.Item)
				{
					i.Value.Item->Use(DeltaTime * GameConstants::ItemConditionDecrease * (i.Value.bRightTool ? 1.f : GameConstants::WrongItemConditionPenalty));
				}
			}
		}
		if (Breaking >= SolidUnitData->BreakingTime)
		{
			Break();
		}
		else if ((SolidUnitData->bUseFlipbook ? SolidUnitData->BreakFlipbooks.Num() : SolidUnitData->BreakSprites.Num()) > 0)
		{
			int NewStage = Breaking *
				(SolidUnitData->bUseFlipbook ? SolidUnitData->BreakFlipbooks.Num() : SolidUnitData->BreakSprites.Num()) /
				SolidUnitData->BreakingTime;
			if (NewStage != BreakingStage)
			{
				BreakingStage = NewStage;
				SetAppearance(NewStage);
			}
		}
	}
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

int ASolidUnit::StartBreaking(AItem* By)
{
	int DestroyerId = Destroyers.Num();
	Destroyers.Add(DestroyerId, FDestroyerInfo());
	SwitchItem(DestroyerId, By);
	return DestroyerId;
}

void ASolidUnit::SwitchItem(int DestroyerId, AItem* By)
{
	if (Destroyers.Contains(DestroyerId))
	{
		FDestroyerInfo Info;
		Info.Item = By;
		if (By)
		{
			Info.bRightTool = false;
			for (const FBreakProfileGroup& i : BreakProfileData->CanBeBrokenBy)
			{
				bool bRightForGroup = true;
				for (const FGameplayTag& j : i.Group)
				{
					if (By->GetId() != j && !By->GetItemData().Tags.Contains(j))
					{
						bRightForGroup = false;
						break;
					}
				}
				if (bRightForGroup)
				{
					Info.bRightTool = true;
					break;
				}
			}
			Info.SpeedMultiplier = By->GetItemData().BreakingSpeedMultiplier;
		}
		else
		{
			Info.bRightTool = false;
			Info.SpeedMultiplier = 1.f;
		}
		if (BreakProfileData->CanBeBrokenBy.Num() == 0)
		{
			Info.bRightTool = true;
		}
		if (!Info.bRightTool && !BreakProfileData->bCanBeBrokenByHand)
		{
			Info.SpeedMultiplier = 0.f;
		}
		Destroyers[DestroyerId] = Info;
	}
}

void ASolidUnit::StopBreaking(int DestroyerId)
{
	if (Destroyers.Contains(DestroyerId))
	{
		Destroyers.Remove(DestroyerId);
		if (Destroyers.Num() == 0)
		{
			Breaking = 0.f;
			BreakingStage = -1;
			SetAppearance(-1);
		}
	}
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
//       |     |   |     |              |O|                         //
//       |     |  _|     |              | |    It says to you,      //
//       |     |   |     |              / \           Hello! :)     //
//       |     |   |     |             /| |\                        //
///////////////////////////////////////ROOT!//////////////////////////

void ASolidUnit::Break()
{
	for (TPair<int, FDestroyerInfo>& i : Destroyers)
	{
		if (i.Value.bRightTool)
		{
			for (const FItemDrop& j : SolidUnitData->Drop)
			{
				static int SpawnPosition = 0;
				int Count;
				if ((j.Chance >= 1.f || FMath::RandRange(0.f, 1.f) < j.Chance) && (Count = FMath::RandRange(j.Min, j.Max)) > 0)
				{
					AThrownItem* Drop = GetWorld()->SpawnActor<AThrownItem>(GAME_MODE->GetDatabase()->GetExtraData().ThrownItemClass.Get(), GetActorLocation() + FVector(SolidUnitData->Size, 0.f) * GameConstants::TileSize * FMath::RandRange(-.5f, .5f), FRotator(0.f, 0.f, 0.f));
					Drop->SetItem(GetWorld()->SpawnActor<AItem>(GAME_MODE->GetDatabase()->GetItemData(j.Item).Class.Get()));
				}
			}
			break;
		}
	}

	GetWorld()->DestroyActor(this);
}

void ASolidUnit::SetAppearance(int Stage)
{
	if (SolidUnitData->bUseFlipbook)
	{
		int FlipbookPosition = FlipbookComponent->GetPlaybackPosition();
		FlipbookComponent->SetFlipbook(Stage < 0 ? SolidUnitData->Flipbook : SolidUnitData->BreakFlipbooks[Stage]);
		FlipbookComponent->SetPlaybackPosition(FlipbookPosition, false); // I'm not sure what happens if FlipbookPosition is more than flipbook length
	}
	else
	{
		SpriteComponent->SetSprite(Stage < 0 ? SolidUnitData->Sprite : SolidUnitData->BreakSprites[Stage]);
	}
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
