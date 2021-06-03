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

	UFUNCTION(BlueprintCallable, Category = "General")
	int GetCount() const;

			/* STATS */

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
