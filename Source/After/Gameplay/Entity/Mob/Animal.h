    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Animal.h                    //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Mob.h"

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

public:
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FAnimalInfo& GetAnimalData() const;

protected:
			/* GENERAL */

	const FAnimalInfo* AnimalData;

			/* DAMAGE */

	virtual void Death(FDamageType Type, const AActor* Murderer) override;
	virtual void Disappear() override;

			/* MUTATION */

	UPROPERTY(BlueprintReadOnly, Category = "Mutation")
	bool bShouldMutate;

	UFUNCTION(Category = "Mutation")
	void Mutate();
};
