    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: SolidUnit.cpp               //
////////////////////////////////////////

#include "SolidUnit.h"

#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"

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
}

void ASolidUnit::BeginPlay()
{
	Super::BeginPlay();

	// Get database
	const UDatabase* Database = GAME_MODE->GetDatabase();
	SolidUnitData = &Database->GetSolidUnitData(Id);
	BreakProfileData = &Database->GetBreakProfileData(SolidUnitData->BreakProfile);

	CollisionComponent->SetBoxExtent(GameConstants::TileSize * FVector(SolidUnitData->Size, 1.f));
	if (IsValid(DamageBoxComponent)) // Damage box can be destroyed by AUnit::BeginPlay
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
		FlipbookComponent->DestroyComponent();
		FlipbookComponent = nullptr;

		SpriteComponent->SetSprite(SolidUnitData->Sprite);
	}

	if (UnitData->bSelectable)
	{
		if (Database->GetExtraData().SelectionSprites.Contains(SolidUnitData->Size) ||
			Database->GetExtraData().SelectionSprites[SolidUnitData->Size])
		{
			SelectionSpriteComponent->SetSprite(Database->GetExtraData().SelectionSprites[SolidUnitData->Size]);
		}
		else
		{
			UE_LOG(LogGameplay, Error, TEXT("Database doesn't contain selection sprite with size %dx%d"), SolidUnitData->Size.X, SolidUnitData->Size.Y);
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
			// Entities can use hands. In this cast i.Value.Item == nullptr
			Breaking += DeltaTime * i.Value.SpeedMultiplier;
			if (IsValid(i.Value.Item))
			{
				if (i.Value.bIsToolRight)
				{
					i.Value.Item->Use(DeltaTime * GameConstants::ItemConditionDecrease);
				}
				else
				{
					i.Value.Item->Use(DeltaTime * GameConstants::ItemConditionDecrease * GameConstants::WrongItemConditionPenalty);
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

void ASolidUnit::Interact(ALast* Last)
{
	// This should be pure virtual, but it's Unreal...
}

void ASolidUnit::Damage(float Value, FDamageType Type, AActor* FromWho)
{
	Health -= Value * SolidUnitData->DamageResist[Type];

	if (Health <= 0.f)
	{
		Health = 0.f;
		Kill(Type, FromWho);
	}

	OnDamage.Broadcast(Value, Type, FromWho);
	OnHealthChanged.Broadcast(Health);
}

int ASolidUnit::StartBreaking(AItem* By)
{
	int DestroyerId = Destroyers.Num();
	while (Destroyers.Contains(DestroyerId))
	{
		DestroyerId--;
	}
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
		if (IsValid(By))
		{
			Info.bIsToolRight = false;
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
					Info.bIsToolRight = true;
					break;
				}
			}
			Info.SpeedMultiplier = By->GetItemData().BreakingSpeedMultiplier;
		}
		else
		{
			Info.bIsToolRight = false;
			Info.SpeedMultiplier = 1.f;
		}
		if (BreakProfileData->CanBeBrokenBy.Num() == 0)
		{
			Info.bIsToolRight = true;
		}
		if (!Info.bIsToolRight && !BreakProfileData->bCanBeBrokenByHand)
		{
			Info.SpeedMultiplier = 0.f;
		}
		Destroyers[DestroyerId] = Info;
	}
}

void ASolidUnit::StopBreaking(int DestroyerId)
{
	Destroyers.Remove(DestroyerId);
	if (Destroyers.Num() == 0)
	{
		Breaking = 0.f;
		BreakingStage = -1;
		SetAppearance(-1);
	}
}

void ASolidUnit::Kill(FDamageType Type, AActor* Murderer)
{
	OnDeath.Broadcast(Type, Murderer);
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
		if (i.Value.bIsToolRight)
		{
			for (const FItemDrop& j : SolidUnitData->Drop)
			{
				static int SpawnPosition = 0;
				int Count;
				if ((j.Chance >= 1.f || FMath::RandRange(0.f, 1.f) < j.Chance) && (Count = FMath::RandRange(j.Min, j.Max)) > 0)
				{
					// Todo: Spawn thrown item in special function
					AThrownItem* Drop = GetWorld()->SpawnActor<AThrownItem>(GAME_MODE->GetDatabase()->GetExtraData().ThrownItemClass.Get(), GetActorLocation() + FVector(SolidUnitData->Size, 0.f) * GameConstants::TileSize * FMath::RandRange(-.5f, .5f), FRotator(0.f, 0.f, 0.f));
					Drop->SetItem(GetWorld()->SpawnActor<AItem>(GAME_MODE->GetDatabase()->GetItemData(j.Item).Class.Get()));
				}
			}
			break;
		}
	}

	OnUnitDestroyed.Broadcast();

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
