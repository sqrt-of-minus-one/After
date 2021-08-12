    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Mutant.cpp                  //
////////////////////////////////////////

#include "Mutant.h"

#include "../../LogGameplay.h"
#include "../../../Data/Database/Database.h"
#include "../../../AfterGameModeBase.h"

AMutant::AMutant()
{

}

void AMutant::BeginPlay()
{
	Super::BeginPlay();

	// Get database
	const UDatabase* Database = GAME_MODE->GetDatabase();
	MutantData = &Database->GetMutantData(Id);
}

void AMutant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FMutantInfo& AMutant::GetMutantData() const
{
	return *MutantData;
}
