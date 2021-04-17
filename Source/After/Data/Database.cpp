    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Database.cpp                //
////////////////////////////////////////

#include "Database.h"

void UDatabase::Check()
{
	UE_LOG(LogTemp, Log, TEXT("I'm here!"));
	TArray<FGameplayTag> Tags;

	// Check entity
	EntityData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Entity database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("entity")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not an entity, but it is contained in the entity database"), *i.ToString());
		}
	}

	// Check entity: last
	LastData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Last database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("entity.last")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a last, but it is contained in the last database"), *i.ToString());
		}
	}

	// Check entity: animal
	AnimalData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Animal database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("entity.animal")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not an animal, but it is contained in the animal database"), *i.ToString());
		}
	}

	// Check entity: animal: wolf
	WolfData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Wolf database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("entity.animal.wolf")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a wolf, but it is contained in the wolf database"), *i.ToString());
		}
	}

	// Check entity: mutant
	MutantData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Mutant database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("entity.mutant")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a mutant, but it is contained in the mutant database"), *i.ToString());
		}
	}

	// Check entity: alien
	AlienData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Alien database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("entity.alien")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not an alien, but it is contained in the alien database"), *i.ToString());
		}
	}

	// Check entity: robot
	RobotData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Robot database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("entity.robot")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a robot, but it is contained in the robot database"), *i.ToString());
		}
	}

	// Check unit
	UnitData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Unit database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("unit")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a unit, but it is contained in the unit database"), *i.ToString());
		}
	}

	// Check unit: liquid
	LiquidData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Liquid database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("unit.liquid")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a liquid, but it is contained in the liquid database"), *i.ToString());
		}
	}

	// Check unit: solid unit
	SolidUnitData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Solid unit database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("unit.solid")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a solid unit, but it is contained in the solid unit database"), *i.ToString());
		}
	}

	// Check unit: solid unit: desktop
	DesktopData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Desktop database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("unit.solid.desktop")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a desktop, but it is contained in the desktop database"), *i.ToString());
		}
	}

	// Check unit: solid unit: crate
	CrateData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Crate database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("unit.solid.crate")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a crate, but it is contained in the crate database"), *i.ToString());
		}
	}

	// Check unit: solid unit: vessel
	VesselUnitData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Vessel unit database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("unit.solid.vessel")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a vessel unit, but it is contained in the vessel unit database"), *i.ToString());
		}
	}

	// Check item
	ItemData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Item database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("item")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not an item, but it is contained in the item database"), *i.ToString());
		}
	}

	// Check item: vessel item
	VesselItemData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Vessel item database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("item.vessel")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a vessel item, but it is contained in the vessel item database"), *i.ToString());
		}
	}

	// Check item: buildable
	BuildableData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Buildable item database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("item.buildable")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a buildable item, but it is contained in the buildable item database"), *i.ToString());
		}
	}

	// Check item: fool
	FoodData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Food database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("item.food")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a food, but it is contained in the food database"), *i.ToString());
		}
	}

	// Check item: clothes
	ClothesData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Clothes database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("item.clothes")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a clothes, but it is contained in the clothes database"), *i.ToString());
		}
	}

	// Check item: ranged weapon
	RangedWeaponData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Ranged weapon database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("item.weapon")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a ranged weapon, but it is contained in the ranged weapon database"), *i.ToString());
		}
	}

	// Check item: ammunition
	AmmunitionData.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Ammunition database contains an invalid tag %s"), *i.ToString());
		}
		if (i.ToString().Find(TEXT("item.ammunition")) != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not an ammunition, but it is contained in the ammunition database"), *i.ToString());
		}
	}
}

		/* ENTITY */

const FEntityInfo& UDatabase::GetEntityData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !EntityData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about entity %s that is not contained in the database"), *Tag.ToString());
	}
	return EntityData[Tag];
}

const FLastInfo& UDatabase::GetLastData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !LastData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about Last %s that is not contained in the database"), *Tag.ToString());
	}
	return LastData[Tag];
}

const FMobInfo& UDatabase::GetMobData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !MobData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about mob %s that is not contained in the database"), *Tag.ToString());
	}
	return MobData[Tag];
}

const FAnimalInfo& UDatabase::GetAnimalData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !AnimalData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about animal %s that is not contained in the database"), *Tag.ToString());
	}
	return AnimalData[Tag];
}

const FWolfInfo& UDatabase::GetWolfData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !WolfData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about wolf %s that is not contained in the database"), *Tag.ToString());
	}
	return WolfData[Tag];
}

const FMutantInfo& UDatabase::GetMutantData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !MutantData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about mutant %s that is not contained in the database"), *Tag.ToString());
	}
	return MutantData[Tag];
}

const FAlienInfo& UDatabase::GetAlienData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !AlienData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about alien %s that is not contained in the database"), *Tag.ToString());
	}
	return AlienData[Tag];
}

const FRobotInfo& UDatabase::GetRobotData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !RobotData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about robot %s that is not contained in the database"), *Tag.ToString());
	}
	return RobotData[Tag];
}

		/* UNIT */

const FUnitInfo& UDatabase::GetUnitData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !UnitData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unit %s that is not contained in the database"), *Tag.ToString());
	}
	return UnitData[Tag];
}

const FLiquidInfo& UDatabase::GetLiquidData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !LiquidData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about liquid %s that is not contained in the database"), *Tag.ToString());
	}
	return LiquidData[Tag];
}

const FSolidUnitInfo& UDatabase::GetSolidUnitData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !SolidUnitData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about solid unit %s that is not contained in the database"), *Tag.ToString());
	}
	return SolidUnitData[Tag];
}

const FDesktopInfo& UDatabase::GetDesktopData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !DesktopData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about desktop %s that is not contained in the database"), *Tag.ToString());
	}
	return DesktopData[Tag];
}

const FCrateInfo& UDatabase::GetCrateData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !CrateData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about crate %s that is not contained in the database"), *Tag.ToString());
	}
	return CrateData[Tag];
}

const FVesselUnitInfo& UDatabase::GetVesselUnitData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !VesselUnitData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about vessel unit %s that is not contained in the database"), *Tag.ToString());
	}
	return VesselUnitData[Tag];
}

		/* ITEM */

const FItemInfo& UDatabase::GetItemData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ItemData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about item %s that is not contained in the database"), *Tag.ToString());
	}
	return ItemData[Tag];
}

const FVesselItemInfo& UDatabase::GetVesselItemData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !VesselItemData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about vessel item %s that is not contained in the database"), *Tag.ToString());
	}
	return VesselItemData[Tag];
}

const FBuildableInfo& UDatabase::GetBuildableData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !BuildableData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about buildable item %s that is not contained in the database"), *Tag.ToString());
	}
	return BuildableData[Tag];
}

const FFoodInfo& UDatabase::GetFoodData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !FoodData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about food %s that is not contained in the database"), *Tag.ToString());
	}
	return FoodData[Tag];
}

const FClothesInfo& UDatabase::GetClothesData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ClothesData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about clothes %s that is not contained in the database"), *Tag.ToString());
	}
	return ClothesData[Tag];
}

const FRangedWeaponInfo& UDatabase::GetRangedWeaponData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !RangedWeaponData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about ranged weapon %s that is not contained in the database"), *Tag.ToString());
	}
	return RangedWeaponData[Tag];
}

const FAmmunitionInfo& UDatabase::GetAmmunitionData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !AmmunitionData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about ammunition %s that is not contained in the database"), *Tag.ToString());
	}
	return AmmunitionData[Tag];
}
