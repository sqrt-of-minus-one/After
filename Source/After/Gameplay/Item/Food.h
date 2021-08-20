    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Food.h                      //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Item.h"

#include "Food.generated.h"

UCLASS()
class AFTER_API AFood : public AItem
{
	GENERATED_BODY()
	
public:
	AFood();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

// ==================================================

public:
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FFoodInfo& GetFoodData() const;

			/* INTERACTION */

	// Return true if the item is interactive and interaction has been successfully handled
	virtual bool Interact(ALast* Last);

protected:
			/* GENERAL */

	const FFoodInfo* FoodData;

};
