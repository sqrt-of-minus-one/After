    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Crate.cpp                   //
////////////////////////////////////////

#include "Crate.h"

#include "../../../Components/Inventory/InventoryComponent.h"
#include "../../../Data/Database/Database.h"
#include "../../../Gui/WidgetInitializer.h"
#include "../../LogGameplay.h"
#include "../../../AfterGameModeBase.h"
#include "../../../GameConstants.h"

ACrate::ACrate()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

void ACrate::BeginPlay()
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
	CrateData = &Database->GetCrateData(Id);

	InventoryComponent->Init(CrateData->InventorySize, this);
}

void ACrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ACrate::Interact(ALast* Last)
{
	Super::Interact(Last);

	GAME_MODE->GetWidgetInitializer()->DisplayCrateInventoryWidget(this, Last);

	return true;
}

const FCrateInfo& ACrate::GetCrateData() const
{
	return *CrateData;
}

UInventoryComponent* ACrate::GetInventory() const
{
	return InventoryComponent;
}

void ACrate::Broken()
{
	InventoryComponent->ThrowAll();

	Super::Broken();
}
