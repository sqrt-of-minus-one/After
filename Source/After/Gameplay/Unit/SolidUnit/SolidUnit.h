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

			/* INTERACTION */

	UFUNCTION()
	virtual void Interact();

			/* DAMAGE */

	virtual void Damage(float Value, FDamageType Type, const AActor* FromWho);

	void StartBreaking(/* const UItem* By */);

	void StopBreaking();

protected:
			/* GENERAL */

	const FSolidUnitInfo* SolidUnitData;

	UFUNCTION(BlueprintCallable, Category = "General")
	const FSolidUnitInfo& GetSolidUnitData() const;

			/* STATS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Health;

			/* DAMAGE */

	UFUNCTION()
	virtual void Kill(FDamageType Type, const AActor* Murderer);

	UFUNCTION()
	void Break(/* const UItem* By */);

			/* AUDIO */
	
	UFUNCTION()
	void PlaySound(FSolidUnitSoundType Sound);

			/* COMPONENTS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* BreakSpriteComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UMeshComponent* MeshComponent;
};
