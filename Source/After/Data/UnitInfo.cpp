    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: UnitInfo.cpp                //
////////////////////////////////////////

#include "UnitInfo.h"

void Check(const FUnitInfo& Data, const FGameplayTag& Tag)
{
	// General
	if (Data.Name.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Unit %s doesn't have a name"), *Tag.ToString());
	}
	if (Data.Description.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Unit %s doesn't have a description"), *Tag.ToString());
	}

	// Appearance
	if (Data.Opacity < 0 || Data.Opacity > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Unit %s have invalid value of opacity (%f). It must be between 0 and 1"), *Tag.ToString(), Data.Opacity);
	}
}

void Check(const FLiquidInfo& Data, const FGameplayTag& Tag)
{
	// General
	for (const FGameplayTag& i : Data.Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Liquid %s contains an invalid liquid tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.liquid"))
		{
			UE_LOG(LogTemp, Error, TEXT("Liquid %s contains a tag with invalid name (%s is not a liquid tag)"), *Tag.ToString(), *i.ToString());
		}
	}

	// Flow
	if (Data.Speed <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Liquid %s have non-positive speed (%f)"), *Tag.ToString(), Data.Speed);
	}
	if (Data.EntitySpeedMultiplier < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Liquid %s have negative entity speed multiplier (%f)"), *Tag.ToString(), Data.EntitySpeedMultiplier);
	}

	// Appearance
	if (!Data.StayFlipbook || !Data.SideFlowFlipbook || !Data.DiagonalFlowFlipbook)
	{
		UE_LOG(LogTemp, Error, TEXT("There are flipbooks that liquid %s doesn't have"), *Tag.ToString());
	}

	// Audio
	for_enum<FLiquidSoundType>([&Data, &Tag](FLiquidSoundType i, bool& out_continue)
	{
		if (!Data.Sounds.Sounds.Contains(i))
		{
			UE_LOG(LogTemp, Error, TEXT("There are sound types that liquid %s doesn't have (they should be added but may be empty)"), *Tag.ToString());
			out_continue = false;
		}
	});
}

void Check(const FSolidUnitInfo& Data, const FGameplayTag& Tag)
{
	// General
	for (const FGameplayTag& i : Data.Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Solid unit %s contains an invalid liquid tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.solid"))
		{
			UE_LOG(LogTemp, Error, TEXT("Solid unit %s contains a tag with invalid name (%s is not a solid unit tag)"), *Tag.ToString(), *i.ToString());
		}
	}

	// Stats
	if (Data.MaxHealth <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Solid unit %s has non-positive maximum health (%f)"), *Tag.ToString(), Data.MaxHealth);
	}
	for_enum<FDamageType>([&Data, &Tag](FDamageType i, bool& out_continue)
	{
		if (!Data.DamageResist.Contains(i))
		{
			UE_LOG(LogTemp, Error, TEXT("Solid unit %s doesn't have one of damage resist values (#%d)"), *Tag.ToString(), i);
		}
		else if (Data.DamageResist[i] == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("One of damage resist values (#%d) of solid unit %s is zero (may cause divide by zero)"), i, *Tag.ToString());
		}
	});

	// Appearance
	if (!(Data.bUseFlipbook ? static_cast<bool>(Data.Flipbook) : static_cast<bool>(Data.Sprite)))
	{
		UE_LOG(LogTemp, Error, TEXT("Solid unit %s doesn't have sprite or flipbook"), *Tag.ToString());
	}

	// Breaking
	if (!Data.BreakeProfile.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Solid unit %s has an invalid breake profile (%s)"), *Tag.ToString(), *Data.BreakeProfile.ToString());
	}
	if (!IS_TAG_PARENT(Data.BreakeProfile, "profile.breake"))
	{
		UE_LOG(LogTemp, Error, TEXT("Solid unit %s has a breake profile with invalid name (%s is not a breake profile)"), *Tag.ToString(), *Data.BreakeProfile.ToString());
	}
	if (Data.BreakingTime < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Solid unit %s has negative breaking time (%f)"), *Tag.ToString(), Data.BreakingTime);
	}

	// Audio
	for_enum<FSolidUnitSoundType>([&Data, &Tag](FSolidUnitSoundType i, bool& out_continue)
	{
		if (!Data.Sounds.Sounds.Contains(i))
		{
			UE_LOG(LogTemp, Error, TEXT("There are sound types that liquid %s doesn't have (they should be added but may be empty)"), *Tag.ToString());
			out_continue = false;
		}
	});
}

void Check(const FDesktopInfo& Data, const FGameplayTag& Tag)
{
	// Crafting
	if (Data.SpeedMultiplier <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Desktop %s has non-positive speed multiplier (%f)"), *Tag.ToString(), Data.SpeedMultiplier);
	}

	// Inventory
	if (Data.IngredientsSize <= 0 || Data.ProductsSize <= 0 || Data.FuelSize < 0 || Data.ExtraItemsSize < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Desktop %s has wrong values of inventory size (%d, %d, %d, %d)"),
			*Tag.ToString(), Data.IngredientsSize, Data.ProductsSize, Data.FuelSize, Data.ExtraItemsSize);
	}
}

void Check(const FCrateInfo& Data, const FGameplayTag& Tag)
{
	// Inventory
	if (Data.InventorySize <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Crate %s has non-positive inventory size (%d)"), *Tag.ToString(), Data.InventorySize);
	}
}

void Check(const FVesselUnitInfo& Data, const FGameplayTag& Tag)
{
	// Content
	if (Data.Volume <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Vessel unit %s has non-positive volume (%f)"), *Tag.ToString(), Data.Volume);
	}
	if (!Data.VesselProfile.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Vessel unit %s contains an invalid vessel profile (%s)"), *Tag.ToString(), *Data.VesselProfile.ToString());
	}
	if (!IS_TAG_PARENT(Data.VesselProfile, "tag.solid"))
	{
		UE_LOG(LogTemp, Error, TEXT("Vessel unit %s contains a vessel profile with invalid name (%s is not a vessel profile)"), *Tag.ToString(), *Data.VesselProfile.ToString());
	}
}
