    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: ItemInfo.cpp                //
////////////////////////////////////////

#include "ItemInfo.h"

void Check(const FItemInfo& Data, const FGameplayTag& Tag)
{
	// General
	if (Data.Name.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Item %s doesn't have a name"), *Tag.ToString());
	}
	if (Data.Description.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Item %s doesn't have a description"), *Tag.ToString());
	}
	for (const FGameplayTag& i : Data.Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Item %s contains an invalid item tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.item"))
		{
			UE_LOG(LogTemp, Error, TEXT("Item %s contains a tag with invalid name (%s is not an item tag)"), *Tag.ToString(), *i.ToString());
		}
	}
	
	// Inventory
	if (Data.StackSize <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Item %s has non-positive stack size (%d)"), *Tag.ToString(), Data.StackSize);
	}
	
	// Fuel
	if (Data.Heat < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Item %s has negative heat (%f)"), *Tag.ToString(), Data.Heat);
	}

	// Appearance
	if (!(Data.bUseFlipbook ? static_cast<bool>(Data.Flipbook) : static_cast<bool>(Data.Sprite)))
	{
		UE_LOG(LogTemp, Error, TEXT("Item %s doesn't have sprite or flipbook"), *Tag.ToString());
	}
}

void Check(const FVesselItemInfo& Data, const FGameplayTag& Tag)
{
	// Content
	if (Data.Volume <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Vessel item %s has non-positive volume (%f)"), *Tag.ToString(), Data.Volume);
	}
	if (!Data.VesselProfile.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Vessel item %s contains an invalid vessel profile (%s)"), *Tag.ToString(), *Data.VesselProfile.ToString());
	}
	if (!IS_TAG_PARENT(Data.VesselProfile, "tag.solid"))
	{
		UE_LOG(LogTemp, Error, TEXT("Vessel item %s contains a vessel profile with invalid name (%s is not a vessel profile)"), *Tag.ToString(), *Data.VesselProfile.ToString());
	}
}

void Check(const FBuildableInfo& Data, const FGameplayTag& Tag)
{
	if (!Data.Unit.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Buildable item %s contains an invalid unit (%s)"), *Tag.ToString(), *Data.Unit.ToString());
	}
	if (!IS_TAG_PARENT(Data.Unit, "tag.solid"))
	{
		UE_LOG(LogTemp, Error, TEXT("Buildable item %s contains a unit with invalid name (%s is not a unit)"), *Tag.ToString(), *Data.Unit.ToString());
	}
}

void Check(const FFoodInfo& Data, const FGameplayTag& Tag)
{
	// Using
	if (Data.Satiety <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Food %s has non-positive satiety (%f)"), *Tag.ToString(), Data.Satiety);
	}
}

void Check(const FClothesInfo& Data, const FGameplayTag& Tag)
{
	// Characteristics
	if (Data.Volume <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Clothes %s has non-positive inventory size (%d)"), *Tag.ToString(), Data.Volume);
	}
	for_enum<FDamageType>([&Data, &Tag](FDamageType i, bool& out_continue)
	{
		if (!Data.Resist.Contains(i))
		{
			UE_LOG(LogTemp, Error, TEXT("Clothes %s doesn't have one of damage resist values (#%d)"), *Tag.ToString(), i);
		}
		else if (Data.Resist[i] == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("One of damage resist values (#%d) of clothes %s is zero (may cause divide by zero)"), i, *Tag.ToString());
		}
	});
}

void Check(const FRangedWeaponInfo& Data, const FGameplayTag& Tag)
{
	// Characteristics
	if (Data.DamageMultiplier <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s has non-positive damage multiplier (%f)"), *Tag.ToString(), Data.DamageMultiplier);
	}
	if (Data.FiringRate <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s has non-positive firing rate (%f)"), *Tag.ToString(), Data.FiringRate);
	}
	if (Data.ReloadTime < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s has negative reload time (%f)"), *Tag.ToString(), Data.ReloadTime);
	}
	if (!Data.AmmunitionProfile.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s contains an invalid ammunition profile (%s)"), *Tag.ToString(), *Data.AmmunitionProfile.ToString());
	}
	if (!IS_TAG_PARENT(Data.AmmunitionProfile, "tag.solid"))
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s contains a ammunition profile with invalid name (%s is not a ammunition profile)"), *Tag.ToString(), *Data.AmmunitionProfile.ToString());
	}
	if (Data.DistanceMultiplier <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s has non-positive distance multiplier (%f)"), *Tag.ToString(), Data.DistanceMultiplier);
	}
	if (Data.Accuracy <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s has non-positive accuracy value (%f)"), *Tag.ToString(), Data.Accuracy);
	}
}

void Check(const FAmmunitionInfo& Data, const FGameplayTag& Tag)
{
	// General
	for (const FGameplayTag& i : Data.Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Ammunition %s contains an invalid ammunition tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.item"))
		{
			UE_LOG(LogTemp, Error, TEXT("Ammunition %s contains a tag with invalid name (%s is not an ammunition tag)"), *Tag.ToString(), *i.ToString());
		}
	}

	// Characteristics
	if (!Data.Projectile.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Ammunition %s contains an invalid projectile (%s)"), *Tag.ToString(), *Data.Projectile.ToString());
	}
	if (!IS_TAG_PARENT(Data.Projectile, "tag.item"))
	{
		UE_LOG(LogTemp, Error, TEXT("Ammunition %s contains a projectile with invalid name (%s is not a projectile)"), *Tag.ToString(), *Data.Projectile.ToString());
	}
	if (Data.FiringRateMultiplier <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Ammunition %s has non-positive firing rate multiplier (%f)"), *Tag.ToString(), Data.FiringRateMultiplier);
	}
	if (Data.AccuracyMultiplier <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Ammunition %s has non-positive accuracy multiplier (%f)"), *Tag.ToString(), Data.AccuracyMultiplier);
	}
	if (Data.MagazineSize <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Ammunition %s has non-positive magazine size (%d)"), *Tag.ToString(), Data.MagazineSize);
	}
}
