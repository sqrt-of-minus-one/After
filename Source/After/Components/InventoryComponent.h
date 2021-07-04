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

	void Init(int Size);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const AItem* Get(int Index) const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AItem* Remove(int Index, int Count);
	
	// Returns how many items were put into inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int PutTo(AItem* Item, int Index);
	
	// Returns how many items were put into inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int Put(AItem* Item);

protected:
			/* INVENTORY */

	UPROPERTY(Category = "Inventory")
	bool bInitialized;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TArray<AItem*> Inventory;
};
