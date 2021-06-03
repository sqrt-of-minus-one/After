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

USTRUCT(BlueprintType)
struct FDestroyerInfo
{
	GENERATED_USTRUCT_BODY()

	AItem* Item;
	bool bRightTool;
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

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetHealth() const;

			/* INTERACTION */

	UFUNCTION(Category = "Interaction")
	virtual void Interact();

			/* DAMAGE */

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void Damage(float Value, FDamageType Type, const AActor* FromWho);

			/* BREAKING */

	// Returns destroyer id
	UFUNCTION(Category = "Damage")
	int StartBreaking(AItem* By);

	UFUNCTION(Category = "Damage")
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
	virtual void Kill(FDamageType Type, const AActor* Murderer);

			/* BREAKING */

	UPROPERTY(BlueprintReadOnly, Category = "Breaking")
	TMap<int, FDestroyerInfo> Destroyers;

	UPROPERTY(BlueprintReadOnly, Category = "Breaking")
	float Breaking;

	UPROPERTY(BlueprintReadOnly, Category = "Breaking")
	int BreakingStage;

	UFUNCTION(Category = "Damage")
	void Break();

			/* APPEARANCE */

	// -1 = is not being breaked
	UFUNCTION(Category = "Appearance")
	void SetAppearance(int Stage);

			/* AUDIO */
	
	UFUNCTION(Category = "Audio")
	void PlaySound(FSolidUnitSoundType Sound);

			/* COMPONENTS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* BreakSpriteComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UMeshComponent* MeshComponent;
};
