    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: UnitInfo.cpp                //
////////////////////////////////////////

#include "UnitInfo.h"

#include "DatabaseInitData.h"
#include "ExtraInfo.h"
#include "LogDatabase.h"

void Check(FUnitInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// Damage
	if (Data.SeemsDangerousDelta < 0.f)
	{
		UE_LOG(LogDatabase, Error, TEXT("Unit %s have negative value of seems dangerous delta (%f)"), *Tag.ToString(), Data.Opacity);
	}

	// Appearance
	if (Data.Opacity < 0.f || Data.Opacity > 1.f)
	{
		UE_LOG(LogDatabase, Error, TEXT("Unit %s have invalid value of opacity (%f). It must be between 0 and 1"), *Tag.ToString(), Data.Opacity);
	}
}

void Check(FLiquidInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// General
	for (const FGameplayTag& i : Data.Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogDatabase, Fatal, TEXT("Liquid %s contains an invalid liquid tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.liquid"))
		{
			UE_LOG(LogDatabase, Fatal, TEXT("Liquid %s contains a tag with invalid name (%s is not a liquid tag)"), *Tag.ToString(), *i.ToString());
		}
	}

	// Flow
	if (Data.Speed <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Liquid %s have non-positive speed (%f)"), *Tag.ToString(), Data.Speed);
	}
	if (Data.EntitySpeedMultiplier < 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Liquid %s have negative entity speed multiplier (%f)"), *Tag.ToString(), Data.EntitySpeedMultiplier);
	}

	// Appearance
	bool bError = false;
	for_enum<FLiquidStatus>([&Data, &Tag, &InitData, &ExtraData, &bError](FLiquidStatus i, bool& out_continue)
	{
		if (!Data.Flipbooks.Contains(i))
		{
			Data.Flipbooks.Add(i);
		}
		if (!Data.Flipbooks[i])
		{
			bError = true;
			InitData.LiquidReplaced.AddTail({ Tag, i });
			Data.Flipbooks[i] = ExtraData.DebugLiquidFlipbooks[i];
		}
	});
	if (bError)
	{
		UE_LOG(LogDatabase, Error, TEXT("There are flipbooks that liquid %s doesn't have"), *Tag.ToString());
	}

	// Audio
	bError = false;
	for_enum<FLiquidSoundType>([&Data, &Tag, &bError](FLiquidSoundType i, bool& out_continue)
	{
		if (!Data.Sounds.Sounds.Contains(i))
		{
			Data.Sounds.Sounds.Add(i);
		}
	});
	if (bError)
	{
		UE_LOG(LogDatabase, Warning, TEXT("There are sound types that liquid %s doesn't have (they were added)"), *Tag.ToString());
	}
}

void Check(FSolidUnitInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// General
	for (const FGameplayTag& i : Data.Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogDatabase, Fatal, TEXT("Solid unit %s contains an invalid liquid tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.solid"))
		{
			UE_LOG(LogDatabase, Fatal, TEXT("Solid unit %s contains a tag with invalid name (%s is not a solid unit tag)"), *Tag.ToString(), *i.ToString());
		}
	}

	// Stats
	if (Data.MaxHealth <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Solid unit %s has non-positive maximum health (%f)"), *Tag.ToString(), Data.MaxHealth);
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
		UE_LOG(LogDatabase, Warning, TEXT("Solid unit %s didn't have some damage resist values (they was added)"), *Tag.ToString());
	}
	if (Zero)
	{
		UE_LOG(LogDatabase, Error, TEXT("Some damage resist values of solid unit %s are zero (may cause divide by zero)"), *Tag.ToString());
	}

	// Appearance
	if (!(Data.bUseFlipbook ? static_cast<bool>(Data.Flipbook) : static_cast<bool>(Data.Sprite)))
	{
		UE_LOG(LogDatabase, Error, TEXT("Solid unit %s doesn't have sprite or flipbook (also check Use Flipbook flag)"), *Tag.ToString());
		InitData.SolidUnitReplaced.AddTail({ Tag });
		if (Data.bUseFlipbook)
		{
			Data.Flipbook = ExtraData.DebugSolidUnitFlipbook;
		}
		else
		{
			Data.Sprite = ExtraData.DebugSolidUnitSprite;
		}
	}
	if (Data.Size.X <= 0 || Data.Size.Y <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Solid unit %s have non-positive size (%d, %d)"), *Tag.ToString(), Data.Size.X, Data.Size.Y);
	}

	// Breaking
	if (!Data.BreakProfile.IsValid())
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Solid unit %s has an invalid break profile (%s)"), *Tag.ToString(), *Data.BreakProfile.ToString());
	}
	if (!IS_TAG_PARENT(Data.BreakProfile, "profile.break"))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Solid unit %s has a break profile with invalid name (%s is not a break profile)"), *Tag.ToString(), *Data.BreakProfile.ToString());
	}
	if (Data.BreakingTime < 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Solid unit %s has negative breaking time (%f)"), *Tag.ToString(), Data.BreakingTime);
	}

	// Audio
	bool bError = false;
	for_enum<FSolidUnitSoundType>([&Data, &Tag, &bError](FSolidUnitSoundType i, bool& out_continue)
	{
		if (!Data.Sounds.Sounds.Contains(i))
		{
			Data.Sounds.Sounds.Add(i);
		}
	});
	if (bError)
	{
		UE_LOG(LogDatabase, Warning, TEXT("There are sound types that solid unit %s doesn't have (they were added)"), *Tag.ToString());
	}
}

void Check(FDesktopInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// Crafting
	if (Data.SpeedMultiplier <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Desktop %s has non-positive speed multiplier (%f)"), *Tag.ToString(), Data.SpeedMultiplier);
	}

	// Inventory
	if (Data.IngredientsSize <= 0 || Data.ProductsSize <= 0 || Data.FuelSize < 0 || Data.ExtraItemsSize < 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Desktop %s has wrong values of inventory size (%d, %d, %d, %d)"),
			*Tag.ToString(), Data.IngredientsSize, Data.ProductsSize, Data.FuelSize, Data.ExtraItemsSize);
	}
}

void Check(FCrateInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// Inventory
	if (Data.InventorySize <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Crate %s has non-positive inventory size (%d)"), *Tag.ToString(), Data.InventorySize);
	}
}

void Check(FVesselUnitInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// Content
	if (Data.Volume <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Vessel unit %s has non-positive volume (%f)"), *Tag.ToString(), Data.Volume);
	}
	if (!Data.VesselProfile.IsValid())
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Vessel unit %s contains an invalid vessel profile (%s)"), *Tag.ToString(), *Data.VesselProfile.ToString());
	}
	if (!IS_TAG_PARENT(Data.VesselProfile, "tag.solid"))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Vessel unit %s contains a vessel profile with invalid name (%s is not a vessel profile)"), *Tag.ToString(), *Data.VesselProfile.ToString());
	}
}
