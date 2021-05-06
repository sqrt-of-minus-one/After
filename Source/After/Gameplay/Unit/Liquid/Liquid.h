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
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FLiquidInfo& GetLiquidData() const;

			/* CHANGE */
	
	// Tries to add some liquid. Returns how much liquid was added
	UFUNCTION(Category = "Change")
	float Add(FGameplayTag Liquid, float AddedAmount);

	// Tries to take some liquid into the vessel. Returns how much liquid was taken
	UFUNCTION(Category = "Change")
	float Get(FGameplayTag Liquid, float GotAmount);

			/* FLOW */

	UFUNCTION(Category = "Change")
	void BeginFlow();

protected:
	void ClearTimers(AActor* Actor, EEndPlayReason::Type Reason);

			/* GENERAL */

	const FLiquidInfo* LiquidData;

			/* STATUS */
	
	UPROPERTY(BlueprintReadOnly, Category = "Status")
	float Amount;

			/* FLOW */
	
	UPROPERTY(BlueprintReadOnly, Category = "Flow")
	FTimerHandle FlowTimer;

	UFUNCTION()
	void Flow();

			/* AUDIO */
	
	UFUNCTION()
	void PlaySound(FLiquidSoundType Sound);
};
