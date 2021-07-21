    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: SolidUnit.h                 //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "../Unit.h"
#include "../../../Data/Database/ProfileInfo.h"

#include "SolidUnit.generated.h"

class AItem;
class ALast;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUnitDeathEvent, FDamageType, Type, AActor*, Murderer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUnitDamageReceivedEvent, float, Value, FDamageType, Type, AActor*, FromWho);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestroyedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnitStatsChangedEvent, float, NewValue);

USTRUCT(BlueprintType)
struct FDestroyerInfo
{
	GENERATED_USTRUCT_BODY()

	AItem* Item;
	bool bIsToolRight;
	float SpeedMultiplier;
};

UCLASS()
class AFTER_API ASolidUnit : public AUnit
{
	GENERATED_BODY()

public:
	ASolidUnit();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

public:
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FSolidUnitInfo& GetSolidUnitData() const;

			/* STATS */

	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FUnitStatsChangedEvent OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetHealth() const;

			/* INTERACTION */

	UFUNCTION(Category = "Interaction")
	virtual void Interact(ALast* Last);

			/* DAMAGE */

	UPROPERTY(BlueprintAssignable, Category = "Damage")
	FUnitDamageReceivedEvent OnDamage;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void Damage(float Value, FDamageType Type, AActor* FromWho);

	UPROPERTY(BlueprintAssignable, Category = "Damage")
	FUnitDeathEvent OnDeath;

			/* BREAKING */

	UPROPERTY(BlueprintAssignable, Category = "Breaking")
	FDestroyedEvent OnUnitDestroyed;

	// Returns destroyer id
	UFUNCTION(Category = "Breaking")
	int StartBreaking(AItem* By);

	UFUNCTION(Category = "Breaking")
	void SwitchItem(int DestroyerId, AItem* By);

	UFUNCTION(Category = "Breaking")
	void StopBreaking(int DestroyerId);

protected:
			/* GENERAL */

	const FSolidUnitInfo* SolidUnitData;
	const FBreakProfileInfo* BreakProfileData;

			/* STATS */

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Health;

			/* DAMAGE */

	UFUNCTION(Category = "Damage")
	virtual void Kill(FDamageType Type, AActor* Murderer);

			/* BREAKING */

	UPROPERTY(BlueprintReadOnly, Category = "Breaking")
	FTimerHandle BreakingTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Breaking")
	TMap<int, FDestroyerInfo> Destroyers;

	UPROPERTY(BlueprintReadOnly, Category = "Breaking")
	float BreakingSpeedMultiplier;

	UPROPERTY(BlueprintReadOnly, Category = "Breaking")
	int BreakingStagesCount;

	UPROPERTY(BlueprintReadOnly, Category = "Breaking")
	int BreakingStage;

	// Setup the timer; BreakingRequiredTime must be set beforehand
	UFUNCTION(Category = "Breaking")
	void Break();

	UFUNCTION(Category = "Breaking")
	void NextBreakingStage();

	// Stop breaking and clear the timer
	UFUNCTION(Category = "Breaking")
	void ResetBreaking();

	UFUNCTION(Category = "Damage")
	void Broken();

			/* APPEARANCE */

	// -1 = is not being broken
	UFUNCTION(Category = "Appearance")
	void SetAppearance();

			/* COMPONENTS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UMeshComponent* MeshComponent;
};
