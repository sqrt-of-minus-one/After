    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Animal.cpp                  //
////////////////////////////////////////

#include "Animal.h"

#include "../../LogGameplay.h"
#include "../../../Data/Database/Database.h"
#include "../../../AfterGameModeBase.h"
#include "Mutant.h"

AAnimal::AAnimal()
{

}

void AAnimal::BeginPlay()
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

void AAnimal::Death(FDamageType Type, const AActor* Murderer)
{
	bShouldMutate = Cast<AMutant>(Murderer) || Type == FDamageType::Radiation;
	Super::Death(Type, Murderer);
}

void AAnimal::Disappear()
{
	if (bShouldMutate)
	{
		Mutate();
	}

	Super::Disappear();
}

void AAnimal::Mutate()
{
	const UDatabase* Database = GAME_MODE->GetDatabase();
	GetWorld()->SpawnActor<AMutant>(Database->GetMutantData(AnimalData->Mutant).Class, GetActorLocation(), GetActorRotation());
}
