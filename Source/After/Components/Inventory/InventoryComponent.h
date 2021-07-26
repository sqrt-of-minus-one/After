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
class UPlayerInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemRemovedEvent, int, Index, AItem*, Item);

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
	
	// Do not change item! Use Take() and Put() methods
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AItem* Get(int Index) const;

	// Returns an index of item with passed tag or -1 if tag wasn't found
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int Find(FGameplayTag ItemTag) const;

	// Remove some items from the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AItem* Take(int Index, int Count);

	// Returns how many items were put into inventory (-1 if inventory is not initialized)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int Put(AItem* Item, int Count);

	// Returns how many items were put into inventory (-1 if inventory is not initialized)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int PutAll(AItem* Item);

	// Returns how many items were moved (-1 if inventory is not initialized)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int MoveToInventory(int Index, int Count, UInventoryComponent* InventoryComponent);

	// Returns how many items were moved (-1 if inventory is not initialized)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int MoveToPlayerInventory(int Index, int Count, UPlayerInventoryComponent* InventoryComponent);

	// Is called when item is removed from the inventory
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FItemRemovedEvent OnItemRemoved;

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

	UFUNCTION(Category = "Inventory")
	void ItemBroken(AItem* Item, float Weight);

private:
	template<typename T>
	int MoveToInventory_(int Index, int Count, T* InventoryComponent);
};
