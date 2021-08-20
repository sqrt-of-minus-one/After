    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Item.cpp                    //
////////////////////////////////////////

#include "Item.h"

#include "../LogGameplay.h"
#include "../../Data/Database/Database.h"
#include "../../AfterGameModeBase.h"

AItem::AItem() :
	Count(1)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AItem::BeginPlay()
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
	ItemData = &Database->GetItemData(Id);

	Condition = ItemData->MaxCondition;
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FItemInfo& AItem::GetItemData() const
{
	return *ItemData;
}

const FGameplayTag& AItem::GetId() const
{
	return Id;
}

int AItem::GetCount() const
{
	return Count;
}

bool AItem::SetCount(int NewCount)
{
	int OldCount = Count;
	if (NewCount <= 0)
	{
		Count = 0;
		OnCountChanged.Broadcast(this, OldCount, 0, ItemData->Weight);
		OnItemCountZero.Broadcast(this, ItemData->Weight);
		Destroy();
		return true;
	}
	else if (ItemData->bIsStackable)
	{
		Count = NewCount;
		OnCountChanged.Broadcast(this, OldCount, NewCount, ItemData->Weight);
		return true;
	}
	else
	{
		return false;
	}
}

float AItem::GetCondition() const
{
	return Condition;
}

void AItem::DecreaseCondition(float ConditionDecrease)
{
	if (ItemData->MaxCondition > 0.f)
	{
		Condition -= ConditionDecrease;
		if (Condition <= 0.f)
		{
			Condition = 0.f;
			OnItemBroken.Broadcast(this, ItemData->Weight);
			Destroy();
		}
		OnConditionChanged.Broadcast(Condition);
	}
}

bool AItem::Interact(ALast* Last)
{
	return false;
}
