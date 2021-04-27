    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Mob.cpp                     //
////////////////////////////////////////

#include "Mob.h"

#include "../../Data/Database.h"
#include "../../AfterGameModeBase.h"

AMob::AMob()
{

}

void AMob::BeginPlay()
{
	Super::BeginPlay();
	
	// Get game mode
	AAfterGameModeBase* GameMode = Cast<AAfterGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Auth game mode is not AAfterGameModeBase"));
	}

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	MobData = &Database->GetMobData(Id);
}

void AMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FMobInfo& AMob::GetMobData() const
{
	return *MobData;
}
