    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Mutant.h                    //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Mob.h"

#include "Mutant.generated.h"

UCLASS()
class AFTER_API AMutant : public AMob
{
	GENERATED_BODY()

public:
	AMutant();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

public:
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FMutantInfo& GetMutantData() const;

protected:
			/* GENERAL */

	const FMutantInfo* MutantData;
};
