    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: SolidUnit.h                 //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "../Unit.h"

#include "SolidUnit.generated.h"

//class UItem;

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

	UFUNCTION(Category = "Damage")
	void StartBreaking(/* const UItem* By */);

	UFUNCTION(Category = "Damage")
	void StopBreaking();

protected:
			/* GENERAL */

	const FSolidUnitInfo* SolidUnitData;

			/* STATS */

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Health;

			/* DAMAGE */

	UFUNCTION(Category = "Damage")
	virtual void Kill(FDamageType Type, const AActor* Murderer);

	UFUNCTION(Category = "Damage")
	void Break(/* const UItem* By */);

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
