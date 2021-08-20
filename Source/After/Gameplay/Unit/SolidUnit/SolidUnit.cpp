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
	BreakingSpeedMultiplier(0.f),
	BreakingStage(-1)
{
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

		BreakingStagesCount = SolidUnitData->BreakFlipbooks.Num();
	}
	else
	{
		MeshComponent = SpriteComponent;

		SelectionSpriteComponent->AttachToComponent(SpriteComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		SelectionSpriteComponent->SetWorldLocation(GetActorLocation());
		FlipbookComponent->DestroyComponent();
		FlipbookComponent = nullptr;

		SpriteComponent->SetSprite(SolidUnitData->Sprite);

		BreakingStagesCount = SolidUnitData->BreakSprites.Num();
	}
	if (BreakingStagesCount <= 0)
	{
		BreakingStagesCount = 1;
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
}

const FSolidUnitInfo& ASolidUnit::GetSolidUnitData() const
{
	return *SolidUnitData;
}

float ASolidUnit::GetHealth() const
{
	return Health;
}

bool ASolidUnit::Interact(ALast* Last)
{
	return false;
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

		BreakingSpeedMultiplier -= Destroyers[DestroyerId].SpeedMultiplier;

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

		BreakingSpeedMultiplier += Info.SpeedMultiplier;

		Break();
	}
}

void ASolidUnit::StopBreaking(int DestroyerId)
{
	BreakingSpeedMultiplier -= Destroyers[DestroyerId].SpeedMultiplier;
	Destroyers.Remove(DestroyerId);
	if (Destroyers.Num() > 0)
	{
		Break();
	}
	else
	{
		ResetBreaking();
	}
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

void ASolidUnit::Kill(FDamageType Type, AActor* Murderer)
{
	OnDeath.Broadcast(Type, Murderer);
	GetWorld()->DestroyActor(this);
}

void ASolidUnit::Break()
{
	if (BreakingSpeedMultiplier >= 0)
	{
		float NewRate = SolidUnitData->BreakingTime / (BreakingSpeedMultiplier * BreakingStagesCount);
		
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	
		float FirstDelay = -1.f;
		if (TimerManager.IsTimerActive(BreakingTimer))
		{
			FirstDelay = TimerManager.GetTimerRemaining(BreakingTimer) * NewRate / TimerManager.GetTimerRate(BreakingTimer);
		}
		else
		{
			NextBreakingStage();
		}

		TimerManager.SetTimer(BreakingTimer, this, &ASolidUnit::NextBreakingStage, NewRate, true, FirstDelay);
	}
}

void ASolidUnit::NextBreakingStage()
{
	if (++BreakingStage >= BreakingStagesCount)
	{
		Broken();
	}
	else
	{
		SetAppearance();
	}
}

void ASolidUnit::ResetBreaking()
{
	GetWorld()->GetTimerManager().ClearTimer(BreakingTimer);
	BreakingStage = -1;
	BreakingSpeedMultiplier = 0.f;
	SetAppearance();
}

void ASolidUnit::Broken()
{
	GetWorld()->GetTimerManager().ClearTimer(BreakingTimer);

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
					SPAWN_THROWN_ITEM(j.Item, Count, RAND_ITEM_POSITION(GetActorLocation()));
				}
			}
			break;
		}
	}

	for (TPair<int, FDestroyerInfo>& i : Destroyers)
	{
		if (IsValid(i.Value.Item))
		{
			i.Value.Item->DecreaseCondition(SolidUnitData->BreakingTime * (i.Value.bIsToolRight ? GameConstants::ItemConditionDecrease : GameConstants::WrongItemConditionPenalty));
		}
	}

	OnUnitDestroyed.Broadcast();

	GetWorld()->DestroyActor(this);
}

void ASolidUnit::SetAppearance()
{
	if (SolidUnitData->bUseFlipbook)
	{
		int FlipbookPosition = FlipbookComponent->GetPlaybackPosition();
		FlipbookComponent->SetFlipbook((BreakingStage < 0 || SolidUnitData->BreakFlipbooks.Num() <= 0) ?
			SolidUnitData->Flipbook : SolidUnitData->BreakFlipbooks[BreakingStage]);
		FlipbookComponent->SetPlaybackPosition(FlipbookPosition, false); // I'm not sure what happens if FlipbookPosition is more than flipbook length
	}
	else
	{
		SpriteComponent->SetSprite((BreakingStage < 0 || SolidUnitData->BreakSprites.Num() <= 0) ?
			SolidUnitData->Sprite : SolidUnitData->BreakSprites[BreakingStage]);
	}
}
