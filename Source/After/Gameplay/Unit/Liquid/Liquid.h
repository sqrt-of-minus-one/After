    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Liquid.h                    //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "../Unit.h"

#include "Liquid.generated.h"

UCLASS()
class AFTER_API ALiquid : public AUnit
{
	GENERATED_BODY()

public:
	ALiquid();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

public:
			/* CHANGE */
	
	// Tries to add some liquid. Returns how much liquid was added
	float Add(FGameplayTag Liquid, float AddedAmount);

	// Tries to take some liquid into the vessel. Returns how much liquid was taken
	float Get(FGameplayTag Liquid, float GotAmount);

			/* FLOW */

	UFUNCTION()
	void BeginFlow();

protected:
			/* GENERAL */

	const FLiquidInfo* LiquidData;

	UFUNCTION(BlueprintCallable, Category = "General")
	const FLiquidInfo& GetLiquidData() const;

			/* STATUS */
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status")
	float Amount;

			/* FLOW */
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flow")
	FTimerHandle FlowTimer;

	UFUNCTION()
	void Flow();

			/* AUDIO */
	
	UFUNCTION()
	void PlaySound(FLiquidSoundType Sound);
};
