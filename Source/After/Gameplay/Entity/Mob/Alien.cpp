    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Alien.cpp                   //
////////////////////////////////////////

#include "Alien.h"

#include "../../LogGameplay.h"
#include "../../../Data/Database/Database.h"
#include "../../../AfterGameModeBase.h"

AAlien::AAlien()
{

}

void AAlien::BeginPlay()
{
	Super::BeginPlay();
	
	// Get game mode
	AAfterGameModeBase* GameMode = Cast<AAfterGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogGameplay, Fatal, TEXT("Auth game mode is not AAfterGameModeBase"));
	}

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	AlienData = &Database->GetAlienData(Id);
}

void AAlien::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FAlienInfo& AAlien::GetAlienData() const
{
	return *AlienData;
}
