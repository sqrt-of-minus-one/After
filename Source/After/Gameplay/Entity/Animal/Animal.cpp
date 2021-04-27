    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Animal.cpp                  //
////////////////////////////////////////

#include "Animal.h"

#include "../../../Data/Database.h"
#include "../../../AfterGameModeBase.h"

AAnimal::AAnimal()
{

}

void AAnimal::BeginPlay()
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
	AnimalData = &Database->GetAnimalData(Id);
}

void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FAnimalInfo& AAnimal::GetAnimalData() const
{
	return *AnimalData;
}

void AAnimal::Mutate()
{

}
