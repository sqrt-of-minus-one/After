    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: WidgetInitializer.cpp       //
////////////////////////////////////////

#include "WidgetInitializer.h"

#include "Blueprint/UserWidget.h"

AWidgetInitializer::AWidgetInitializer() :
	CurrentWidget(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWidgetInitializer::BeginPlay()
{
	Super::BeginPlay();
}

void AWidgetInitializer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWidgetInitializer::DisplayCrateInventoryWidget(ACrate* Crate, ALast* Last)
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), CrateInventoryWidget);
	CrateInventoryInit(Widget, Crate, Last);
	if (IsValid(Widget))
	{
		if (IsValid(CurrentWidget))
		{
			CurrentWidget->RemoveFromParent();
		}
		Widget->AddToViewport(1);
		CurrentWidget = Widget;
	}
}

void AWidgetInitializer::DisplayPlayerMenuWidget(ALast* Last, FMenuType Type)
{
	if (IsValid(CurrentWidget))
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
		return;
	}

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), PlayerMenuWidget);
	PlayerMenuInit(Widget, Last, Type);
	if (IsValid(Widget))
	{
		Widget->AddToViewport(1);
		CurrentWidget = Widget;
	}
}
