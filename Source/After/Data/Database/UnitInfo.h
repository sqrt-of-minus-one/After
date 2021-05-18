    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: UnitInfo.h                  //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "../Types.h"

#include "UnitInfo.generated.h"

class UPaperSprite;
struct FDatabaseInitData;
struct FExtraInfo;
class AUnit;
class ALiquid;
class ASolidUnit;

USTRUCT(BlueprintType)
struct FUnitInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Unit's class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSubclassOf<AUnit> Class;

			/* DAMAGE */

	// Damage that is dealt by the unit
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float Damage = 0.f;

	// Type of the unit's damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FDamageType DamageType;

	// Whether mobs should fear the unit
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	bool bSeemsDangerous;

			/* APPEARANCE */

	// The fraction of light that is not passing through the unit
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	float Opacity = 1.f;

			/* INTERACTION */

	// Whether entity can be selected by player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	bool bSelectable = true;
};
void Check(FUnitInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FLiquidInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Liquid's class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSubclassOf<ALiquid> Class;

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
	TMap<FLiquidStatus, UPaperFlipbook*> Flipbooks;

			/* AUDIO */

	// Liquid's sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	FLiquidSounds Sounds;
};
void Check(FLiquidInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FSolidUnitInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Solid unit's class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSubclassOf<ASolidUnit> Class;

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
	bool bUseFlipbook = false;

	// The flipbook of the unit
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	UPaperFlipbook* Flipbook;

	// The flipbooks of unit being destroyed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TArray<UPaperFlipbook*> BreakFlipbooks;

	// The sprite of the unit
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	UPaperSprite* Sprite;

	// The sprites of unit being destroyed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TArray<UPaperSprite*> BreakSprites;

	// The entity's size
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FIntPoint Size;

			/* BREAKING */

	// What can be used to get unit (<<profile.break>>)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Breaking")
	FGameplayTag BreakProfile;

	// How long unit is being broken (seconds)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Breaking")
	float BreakingTime;

			/* AUDIO */

	// Unit's sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	FSolidUnitSounds Sounds;
};
void Check(FSolidUnitInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

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
void Check(FDesktopInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

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
void Check(FCrateInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

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
void Check(FVesselUnitInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);
