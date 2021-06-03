    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: ItemInfo.cpp                //
////////////////////////////////////////

#include "ItemInfo.h"

#include "DatabaseInitData.h"
#include "ExtraInfo.h"
#include "LogDatabase.h"

void Check(FItemInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// General
	for (const FGameplayTag& i : Data.Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogDatabase, Fatal, TEXT("Item %s contains an invalid item tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.item"))
		{
			UE_LOG(LogDatabase, Fatal, TEXT("Item %s contains a tag with invalid name (%s is not an item tag)"), *Tag.ToString(), *i.ToString());
		}
	}

	// Inventory
	if (Data.StackSize <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Item %s has non-positive stack size (%d)"), *Tag.ToString(), Data.StackSize);
	}

	// Attack
	if (Data.Push < 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Item %s has negative push (%f)"), *Tag.ToString(), Data.Push);
	}
	if (Data.AttackRadius <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Item %s has non-positive attack radius (%f)"), *Tag.ToString(), Data.AttackRadius);
	}
	if (Data.AttackInterval < 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Item %s has negative attack interval (%f)"), *Tag.ToString(), Data.AttackInterval);
	}

	// Breaking
	if (Data.BreakingSpeedMultiplier <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Item %s has non-positive breaking time multiplier (%f)"), *Tag.ToString(), Data.BreakingSpeedMultiplier);
	}

	// Fuel
	if (Data.Heat < 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Item %s has negative heat (%f)"), *Tag.ToString(), Data.Heat);
	}

	// Appearance
	if (!(Data.bUseFlipbook ? static_cast<bool>(Data.InventoryFlipbook) : static_cast<bool>(Data.InventorySprite)))
	{
		UE_LOG(LogDatabase, Error, TEXT("Item %s doesn't have inventory sprite or flipbook (also check Use Flipbook flag)"), *Tag.ToString());
		InitData.ItemReplaced.AddTail({ Tag, false });
		if (Data.bUseFlipbook)
		{
			Data.InventoryFlipbook = ExtraData.DebugItemFlipbook;
		}
		else
		{
			Data.InventorySprite = ExtraData.DebugItemSprite;
		}
	}
	if (!(Data.bUseFlipbook ? static_cast<bool>(Data.WorldFlipbook) : static_cast<bool>(Data.WorldSprite)))
	{
		UE_LOG(LogDatabase, Error, TEXT("Item %s doesn't have world sprite or flipbook (also check Use Flipbook flag)"), *Tag.ToString());
		InitData.ItemReplaced.AddTail({ Tag, true });
		if (Data.bUseFlipbook)
		{
			Data.WorldFlipbook = ExtraData.DebugItemFlipbook;
		}
		else
		{
			Data.WorldSprite = ExtraData.DebugItemSprite;
		}
	}
}

void Check(FVesselItemInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// Content
	if (Data.Volume <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Vessel item %s has non-positive volume (%f)"), *Tag.ToString(), Data.Volume);
	}
	if (!Data.VesselProfile.IsValid())
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Vessel item %s contains an invalid vessel profile (%s)"), *Tag.ToString(), *Data.VesselProfile.ToString());
	}
	if (!IS_TAG_PARENT(Data.VesselProfile, "tag.solid"))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Vessel item %s contains a vessel profile with invalid name (%s is not a vessel profile)"), *Tag.ToString(), *Data.VesselProfile.ToString());
	}
}

void Check(FBuildableInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	if (!Data.Unit.IsValid())
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Buildable item %s contains an invalid unit (%s)"), *Tag.ToString(), *Data.Unit.ToString());
	}
	if (!IS_TAG_PARENT(Data.Unit, "tag.solid"))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Buildable item %s contains a unit with invalid name (%s is not a unit)"), *Tag.ToString(), *Data.Unit.ToString());
	}
}

void Check(FFoodInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// Using
	if (Data.Satiety <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Food %s has non-positive satiety (%f)"), *Tag.ToString(), Data.Satiety);
	}
}

void Check(FClothesInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// Characteristics
	if (Data.Volume <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Clothes %s has non-positive inventory size (%d)"), *Tag.ToString(), Data.Volume);
	}
	bool None = false, Zero = false;
	for_enum<FDamageType>([&Data, &Tag, &None, &Zero](FDamageType i, bool& out_continue)
	{
		if (!Data.DamageResist.Contains(i))
		{
			Data.DamageResist.Add(i, 0.f);
			None = true;
		}
		if (Data.DamageResist[i] == 0.f)
		{
			Zero = true;
		}
	});
	if (None)
	{
		UE_LOG(LogDatabase, Warning, TEXT("Clothes %s didn't have some damage resist values (they was added)"), *Tag.ToString());
	}
	if (Zero)
	{
		UE_LOG(LogDatabase, Error, TEXT("Some damage resist values of clothes %s are zero (may cause divide by zero)"), *Tag.ToString());
	}
}

void Check(FRangedWeaponInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// Characteristics
	if (Data.DamageMultiplier <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Weapon %s has non-positive damage multiplier (%f)"), *Tag.ToString(), Data.DamageMultiplier);
	}
	if (Data.PushMultiplier <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Weapon %s has non-positive push multiplier (%f)"), *Tag.ToString(), Data.PushMultiplier);
	}
	if (Data.FiringRate <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Weapon %s has non-positive firing rate (%f)"), *Tag.ToString(), Data.FiringRate);
	}
	if (Data.ReloadTime < 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Weapon %s has negative reload time (%f)"), *Tag.ToString(), Data.ReloadTime);
	}
	if (!Data.AmmunitionProfile.IsValid())
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Weapon %s contains an invalid ammunition profile (%s)"), *Tag.ToString(), *Data.AmmunitionProfile.ToString());
	}
	if (!IS_TAG_PARENT(Data.AmmunitionProfile, "tag.solid"))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Weapon %s contains a ammunition profile with invalid name (%s is not a ammunition profile)"), *Tag.ToString(), *Data.AmmunitionProfile.ToString());
	}
	if (Data.DistanceMultiplier <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Weapon %s has non-positive distance multiplier (%f)"), *Tag.ToString(), Data.DistanceMultiplier);
	}
	if (Data.Accuracy <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Weapon %s has non-positive accuracy value (%f)"), *Tag.ToString(), Data.Accuracy);
	}
}

void Check(FAmmunitionInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// General
	for (const FGameplayTag& i : Data.Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogDatabase, Fatal, TEXT("Ammunition %s contains an invalid ammunition tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.item"))
		{
			UE_LOG(LogDatabase, Fatal, TEXT("Ammunition %s contains a tag with invalid name (%s is not an ammunition tag)"), *Tag.ToString(), *i.ToString());
		}
	}

	// Characteristics
	if (!Data.Projectile.IsValid())
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Ammunition %s contains an invalid projectile (%s)"), *Tag.ToString(), *Data.Projectile.ToString());
	}
	if (!IS_TAG_PARENT(Data.Projectile, "tag.item"))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Ammunition %s contains a projectile with invalid name (%s is not a projectile)"), *Tag.ToString(), *Data.Projectile.ToString());
	}
	if (Data.FiringRateMultiplier <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Ammunition %s has non-positive firing rate multiplier (%f)"), *Tag.ToString(), Data.FiringRateMultiplier);
	}
	if (Data.AccuracyMultiplier <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Ammunition %s has non-positive accuracy multiplier (%f)"), *Tag.ToString(), Data.AccuracyMultiplier);
	}
	if (Data.MagazineSize <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Ammunition %s has non-positive magazine size (%d)"), *Tag.ToString(), Data.MagazineSize);
	}
}
