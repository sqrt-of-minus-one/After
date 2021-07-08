    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: WidgetInitializer.cpp       //
////////////////////////////////////////

#include "WidgetInitializer.h"

AWidgetInitializer::AWidgetInitializer()
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

TSubclassOf<UUserWidget> AWidgetInitializer::GetCrateInventoryWidget() const
{
	return CrateInventoryWidget;
}
