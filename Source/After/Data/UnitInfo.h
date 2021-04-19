    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: UnitInfo.h                  //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Types.h"

#include "UnitInfo.generated.h"

class UPaperSprite;

USTRUCT(BlueprintType)
struct FUnitInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// User-friendly unit's name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FText Name;

	// Unit's description
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FText Description;

			/* DAMAGE */

	// Damage that is dealt by the unit
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float Damage;

	// Type of the unit's damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FDamageType DamageType;

			/* APPEARANCE */

	// The fraction of light that is not passing through the unit
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	float Opacity;

			/* INTERACTION */

	// Whether entity can be selected by player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	bool bSelectable;
};

USTRUCT(BlueprintType)
struct FLiquidInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Liquid's tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TArray<FGameplayTag> Tags;

			/* FLOW */

	// Updates per second
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flow")
	float Speed;

	// Entity's swimming speed is multiplied by this value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flow")
	float EntitySpeedMultiplier;

			/* APPEARANCE */

	// Still liquid's flipbook
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	UPaperFlipbook* StayFlipbook;

	// Side-flowing liquid's flipbook
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	UPaperFlipbook* SideFlowFlipbook;

	// Diagonal-flowing liquid's flipbook
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	UPaperFlipbook* DiagonalFlowFlipbook;

			/* AUDIO */

	// Liquid's sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	FLiquidSounds Sounds;
};

USTRUCT(BlueprintType)
struct FSolidUnitInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Solid unit's tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TArray<FGameplayTag> Tags;

			/* STATS */

	// Maximum value of the entity's health
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MaxHealth;

	// Values by which damage is multiplies (more than 1 = resist; less than 1 = vulnerability)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FDamageType, float> DamageResist;

			/* APPEARANCE */

	// Whether the unit uses flipbook or sprite
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	bool bUseFlipbook;

	// The unit's flipbook
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	UPaperFlipbook* Flipbook;

	// The unit's sprite
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	UPaperSprite* Sprite;

			/* BREAKING */

	// What can be used to get unit (<<profile.breake>>)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Breaking")
	FGameplayTag BreakeProfile;

	// How long unit is being broken (seconds)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Breaking")
	float BreakingTime;

			/* AUDIO */

	// Unit's sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	FSolidUnitSounds Sounds;
};

USTRUCT(BlueprintType)
struct FDesktopInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* CRAFTING */
	
	// Crafting speed is multiplied by this value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting")
	float SpeedMultiplier;

			/* INVENTORY */
	
	// How many items can be putted as ingredients
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int IngredientsSize;
	
	// How many items can be putted as products
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int ProductsSize;

	// How many items can be putted as fuel
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int FuelSize;

	// How many items can be putted as extra
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int ExtraItemsSize;
};

USTRUCT(BlueprintType)
struct FCrateInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* INVENTORY */

	// The crate's volume
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int InventorySize;
};

USTRUCT(BlueprintType)
struct FVesselUnitInfo
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
