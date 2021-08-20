    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Food.cpp                    //
////////////////////////////////////////

#include "Food.h"

#include "../LogGameplay.h"
#include "../Entity/Last.h"
#include "../../Data/Database/Database.h"
#include "../../AfterGameModeBase.h"

AFood::AFood()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFood::BeginPlay()
{
	Super::BeginPlay();

	// Get game mode
	AAfterGameModeBase* GameMode = GAME_MODE;

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	FoodData = &Database->GetFoodData(Id);
}

void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FFoodInfo& AFood::GetFoodData() const
{
	return *FoodData;
}

bool AFood::Interact(ALast* Last)
{
	if (IsValid(Last) && Last->IncreaseSatiety(FoodData->Satiety))
	{
		SetCount(Count - 1);
		return true;
	}
	else
	{
		return false;
	}
}
