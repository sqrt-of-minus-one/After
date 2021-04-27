    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Mutant.cpp                  //
////////////////////////////////////////

#include "Mutant.h"

#include "../../../Data/Database.h"
#include "../../../AfterGameModeBase.h"

AMutant::AMutant()
{

}

void AMutant::BeginPlay()
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
