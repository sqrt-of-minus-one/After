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

protected:
			/* GENERAL */

	const FAlienInfo* AlienData;

	UFUNCTION(BlueprintCallable, Category = "General")
	const FAlienInfo& GetAlienData() const;
};
