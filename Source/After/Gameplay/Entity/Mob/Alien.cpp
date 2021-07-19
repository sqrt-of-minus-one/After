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

	// Get database
	const UDatabase* Database = GAME_MODE->GetDatabase();
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
