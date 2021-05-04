    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Alien.h                     //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Mob.h"

#include "Alien.generated.h"

UCLASS()
class AFTER_API AAlien : public AMob
{
	GENERATED_BODY()

public:
	AAlien();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

public:
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FAlienInfo& GetAlienData() const;

protected:
			/* GENERAL */

	const FAlienInfo* AlienData;
};
