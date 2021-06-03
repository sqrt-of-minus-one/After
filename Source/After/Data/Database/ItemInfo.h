    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: ItemInfo.h                  //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "../Types.h"

#include "ItemInfo.generated.h"

class UPaperSprite;
struct FDatabaseInitData;
struct FExtraInfo;
class AItem;

USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Item's class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSubclassOf<AItem> Class;

	// Item's tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TArray<FGameplayTag> Tags;

			/* INVENTORY */

	// How many items can be placed in one inventory cell
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int StackSize = 100;

			/* ATTACK */

	// Damage that is dealt by item's owner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float Damage;

	// Type of damage that is dealt by item's owner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	FDamageType DamageType;

	// How hard the item pushes its target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float Push;

	// Maximum distance between attacker and attacked
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRadius;

	// Minimum time between two attacks
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackInterval;

			/* BREAKING */

	// Breaking speed is multiplied by this value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Breaking")
	float BreakingSpeedMultiplier = 1.f;

			/* STATS */

	// Maximum value of item's condition. If negative, item doesn't have condition
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MaxCondition = -1.f;

			/* FUEL */

	// The amount of heat that the item can get as fuel
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fuel")
	float Heat = 0.f;

			/* APPEARANCE */

	// Whether the item uses flipbook or sprite
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	bool bUseFlipbook = false;

	// The item's flipbook when it is in inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	UPaperFlipbook* InventoryFlipbook;

	// The item's flipbook when it is in the world
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	UPaperFlipbook* WorldFlipbook;

	// The item's sprite when it is in the inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	UPaperSprite* InventorySprite;

	// The item's sprite when it is in the world
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	UPaperSprite* WorldSprite;
};
void Check(FItemInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FVesselItemInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* CONTENT */

	// The vessel's volume
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Content")
	float Volume;

	// Which liquids can be in the vessel (<<profile.vessel>>)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Content")
	FGameplayTag VesselProfile;
};
void Check(FVesselItemInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FBuildableInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* USING */

	// The unit that this item creates <<unit>>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Using")
	FGameplayTag Unit;
};
void Check(FBuildableInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FFoodInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* USING */

	// Amount of satieny that this food can recover
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Using")
	float Satiety;
};
void Check(FFoodInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FClothesInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// The clothes' type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FClothesType Type;

			/* CHARACTERISTICS */

	// The damage is multiplied by this value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	float Defence;

	// Size of the clothes' inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	int Volume;

	// Values by which damage is multiplies
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	TMap<FDamageType, float> DamageResist;
};
void Check(FClothesInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FRangedWeaponInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* CHARACTERISTICS */

	// The projectile's damage is multiplied by this value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	float DamageMultiplier;

	// The projectile's push is multiplied by this value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	float PushMultiplier;

	// Projectiles per second
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	float FiringRate;

	// If true, player can just hold down the fire button
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	bool bIsAutomatic;

	// Seconds that reloading takes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	float ReloadTime;

	// Which ammunition can be used with this weapon (<<profile.ammunition>>)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	FGameplayTag AmmunitionProfile;

	// Projectile's maximum distance is multiplied by this value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	float DistanceMultiplier;

	// The coefficient that is used in the probability distribution function
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	float Accuracy;
};
void Check(FRangedWeaponInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FAmmunitionInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Ammunition's tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TArray<FGameplayTag> Tags;

			/* CHARACTERISTICS */

	// The projectile that is used (<<projectile>>)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	FGameplayTag Projectile;

	// Weapon's firing rate is multiplied by this value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	float FiringRateMultiplier;

	// Weapon's accuracy is multiplied by this value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	float AccuracyMultiplier;

	// Projectiles in the magazine
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	int MagazineSize;
};
void Check(FAmmunitionInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);
