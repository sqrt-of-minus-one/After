    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: WidgetInitializer.cpp       //
////////////////////////////////////////

#include "WidgetInitializer.h"

#include "Blueprint/UserWidget.h"

AWidgetInitializer::AWidgetInitializer() :
	CurrentWidget(nullptr),
	bIsMainWidgetCreated(false)
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

void AWidgetInitializer::CloseWidget()
{
	if (IsValid(CurrentWidget))
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

void AWidgetInitializer::DisplayMainWidget(ALast* Last)
{
	if (!bIsMainWidgetCreated)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), MainWidget);
		if (IsValid(Widget))
		{
			MainInit(Widget, Last);
			Widget->AddToViewport(0);
		}
		bIsMainWidgetCreated = true;
	}
}

void AWidgetInitializer::DisplayCrateInventoryWidget(ACrate* Crate, ALast* Last)
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), CrateInventoryWidget);
	if (IsValid(Widget))
	{
		CrateInventoryInit(Widget, Crate, Last);
		CloseWidget();
		Widget->AddToViewport(1);
		CurrentWidget = Widget;
		OnWidgetCreated.Broadcast();
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
	if (IsValid(Widget))
	{
		PlayerMenuInit(Widget, Last, Type);
		Widget->AddToViewport(1);
		CurrentWidget = Widget;
		OnWidgetCreated.Broadcast();
	}
}
