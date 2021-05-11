    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Mob.h                       //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "../Entity.h"

#include "Mob.generated.h"

class AUnit;

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

			/* DAMAGE */
	
	virtual void Damage(float Value, FDamageType Type, float Direction, const AActor* FromWho, float Push = 0.f) override;

	UFUNCTION(Category = "Damage")
	virtual void Danger(const AUnit* Unit);

protected:
			/* GENERAL */

	const FMobInfo* MobData;

			/* EVENTS */

//	UDELEGATE(Category = "Events")
	TDelegate<void(float, const AActor*)> DamageDelegate;
	TDelegate<void(const AUnit*)> DangerDelegate;
};
