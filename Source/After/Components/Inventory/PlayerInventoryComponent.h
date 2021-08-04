    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: PlayerInventoryComponent.h  //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "../../Data/Types.h"

#include "PlayerInventoryComponent.generated.h"

class UInventoryComponent;
class AItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHotbarItemChangedEvent, int, HotbarIndex, AItem*, NewItem);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AFTER_API UPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// ==================================================

public:
			/* INVENTORY */

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void Init(float Size, int HotbarSize);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetFullness() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetMaxFullness() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryComponent* GetInventory() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AItem* GetClothes(FClothesType Type) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryComponent* GetClothesInventory(FClothesType Type) const;

	// Returns how many items were put into inventory (-1 if inventory is not initialized)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int Put(AItem* Item, int Count);

	// Returns how many items were put into inventory (-1 if inventory is not initialized)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int PutAll(AItem* Item);

			/* HOTBAR */

	// Do not change item!
	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	AItem* GetHotbarItem(int Index) const;

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	FGameplayTag GetHotbarItemTag(int Index) const;

	// You cannot put into hotbar items that are in clothes
	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void SetHotbarItem(int HotbarIndex, FGameplayTag ItemTag);

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void ClearHotbarSlot(int HotbarIndex);

	// Is called when one of hotbar items is changed (removed or replaced)
	UPROPERTY(BlueprintAssignable, Category = "Hotbar")
	FHotbarItemChangedEvent OnHotbarItemChanged;

protected:
			/* INVENTORY */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	bool bInitialized;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* Inventory;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TMap<FClothesType, AItem*> Clothes;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TMap<FClothesType, UInventoryComponent*> ClothesInventory;

			/* HOTBAR */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Hotbar")
	TArray<FGameplayTag> Hotbar;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Hotbar")
	TArray<AItem*> HotbarItems;

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void HotbarItemRemoved(int Index, AItem* Item);

};
