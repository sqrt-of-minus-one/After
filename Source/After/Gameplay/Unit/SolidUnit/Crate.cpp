    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Crate.cpp                   //
////////////////////////////////////////

#include "Crate.h"

#include "Blueprint/UserWidget.h"

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

	InventoryComponent->Init(CrateData->InventorySize);
}

void ACrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACrate::Interact(ALast* Last)
{
	Super::Interact(Last);

	AAfterGameModeBase* GameMode = GAME_MODE;
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), GameMode->GetWidgetInitializer()->GetCrateInventoryWidget());
	GameMode->GetWidgetInitializer()->CrateInventoryInit(Widget, this, Last);
	if (Widget)
	{
		Widget->AddToViewport(1);
	}
}

const FCrateInfo& ACrate::GetCrateData() const
{
	return *CrateData;
}

UInventoryComponent* ACrate::GetInventory() const
{
	return InventoryComponent;
}
