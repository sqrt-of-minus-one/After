    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Item.h                      //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "../../Data/Database/ItemInfo.h"

#include "Item.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemBrokenEvent, AItem*, Item, float, Weight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConditionChangedEvent, float, NewCondition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountChangedEvent, int, NewCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemCountZeroEvent, AItem*, Item);

UCLASS()
class AFTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	AItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

// ==================================================

public:
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FItemInfo& GetItemData() const;

	UFUNCTION(BlueprintCallable, Category = "General")
	const FGameplayTag& GetId() const;

	// Is called when items count changed (even if new count is zero)
	UPROPERTY(BlueprintAssignable, Category = "General")
	FCountChangedEvent OnCountChanged;

	// Is called when items count becomes zero and item object is destroyed
	UPROPERTY(BlueprintAssignable, Category = "General")
	FItemCountZeroEvent OnItemCountZero;

	UFUNCTION(BlueprintCallable, Category = "General")
	int GetCount() const;

	// Returns true if count was successfully changed
	UFUNCTION(BlueprintCallable, Category = "General")
	bool SetCount(int NewCount);

			/* STATS */

	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FConditionChangedEvent OnConditionChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FItemBrokenEvent OnItemBroken;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetCondition() const;

	UFUNCTION(Category = "Stats")
	void Use(float ConditionDecrease);

protected:
			/* GENERAL */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FGameplayTag Id;

	const FItemInfo* ItemData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	int Count;

			/* STATS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Condition;

};
