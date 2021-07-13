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
	void Init(float Size);

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

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int Put(AItem* Item);

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

};
