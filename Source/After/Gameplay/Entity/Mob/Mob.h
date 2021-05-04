    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Mob.h                       //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "../Entity.h"

#include "Mob.generated.h"

UCLASS()
class AFTER_API AMob : public AEntity
{
	GENERATED_BODY()

public:
	AMob();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

public:
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FMobInfo& GetMobData() const;

protected:
			/* GENERAL */

	const FMobInfo* MobData;
};
