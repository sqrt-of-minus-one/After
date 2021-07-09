    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: InventoryComponent.h        //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "InventoryComponent.generated.h"

class AItem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AFTER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ==================================================

public:
			/* INVENTORY */

	void Init(float Size);

	// Current number of cells
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int GetCurrentSize() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetFullness() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetMaxFullness() const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AItem* Get(int Index) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AItem* Take(int Index, int Count);
	
	// Returns how many items were put into inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int Put(AItem* Item);

protected:
			/* INVENTORY */

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	bool bInitialized;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	float Fullness;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	float MaxFullness;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TArray<AItem*> Inventory;
};
