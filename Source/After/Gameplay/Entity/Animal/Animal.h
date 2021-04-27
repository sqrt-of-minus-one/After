    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Animal.h                    //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "../Mob.h"

#include "Animal.generated.h"

UCLASS()
class AFTER_API AAnimal : public AMob
{
	GENERATED_BODY()

public:
	AAnimal();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

protected:
			/* GENERAL */

	const FAnimalInfo* AnimalData;

	UFUNCTION(BlueprintCallable, Category = "General")
	const FAnimalInfo& GetAnimalData() const;

private:
			/* MUTATION */

	void Mutate();
};
