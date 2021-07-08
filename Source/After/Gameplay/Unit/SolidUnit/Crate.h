    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Crate.h                     //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "SolidUnit.h"

#include "Crate.generated.h"

class UInventoryComponent;

UCLASS()
class AFTER_API ACrate : public ASolidUnit
{
	GENERATED_BODY()

public:
	ACrate();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

public:
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FCrateInfo& GetCrateData() const;

			/* INTERACTION */

	virtual void Interact() override;

			/* CONTENT */

	UFUNCTION(BlueprintCallable, Category = "Content")
	UInventoryComponent* GetInventory() const;

protected:
			/* GENERAL */

	const FCrateInfo* CrateData;

			/* COMPONENTS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* InventoryComponent;
};
