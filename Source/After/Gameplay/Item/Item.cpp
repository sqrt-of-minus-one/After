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

float AItem::GetCondition() const
{
	return Condition;
}