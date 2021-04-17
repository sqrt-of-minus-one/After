    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Database.cpp                //
////////////////////////////////////////

#include "Database.h"

void UDatabase::Check()
{

}

		/* ENTITY */

const FEntityInfo& UDatabase::GetEntityData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !EntityData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting entity %s"), *Tag.ToString());
	}
	return EntityData[Tag];
}

const FLastInfo& UDatabase::GetLastData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !LastData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting Last %s"), *Tag.ToString());
	}
	return LastData[Tag];
}

const FMobInfo& UDatabase::GetMobData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !MobData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting mob %s"), *Tag.ToString());
	}
	return MobData[Tag];
}

const FAnimalInfo& UDatabase::GetAnimalData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !AnimalData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting animal %s"), *Tag.ToString());
	}
	return AnimalData[Tag];
}

const FWolfInfo& UDatabase::GetWolfData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !WolfData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting wolf %s"), *Tag.ToString());
	}
	return WolfData[Tag];
}

const FMutantInfo& UDatabase::GetMutantData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !MutantData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting mutant %s"), *Tag.ToString());
	}
	return MutantData[Tag];
}

const FAlienInfo& UDatabase::GetAlienData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !AlienData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting alien %s"), *Tag.ToString());
	}
	return AlienData[Tag];
}

const FRobotInfo& UDatabase::GetRobotData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !RobotData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting robot %s"), *Tag.ToString());
	}
	return RobotData[Tag];
}

		/* UNIT */

const FUnitInfo& UDatabase::GetUnitData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !UnitData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting unit %s"), *Tag.ToString());
	}
	return UnitData[Tag];
}

const FLiquidInfo& UDatabase::GetLiquidData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !LiquidData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting liquid %s"), *Tag.ToString());
	}
	return LiquidData[Tag];
}

const FSolidUnitInfo& UDatabase::GetSolidUnitData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !SolidUnitData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting solid unit %s"), *Tag.ToString());
	}
	return SolidUnitData[Tag];
}

const FDesktopInfo& UDatabase::GetDesktopData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !DesktopData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting desktop %s"), *Tag.ToString());
	}
	return DesktopData[Tag];
}

const FCrateInfo& UDatabase::GetCrateData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !CrateData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting crate %s"), *Tag.ToString());
	}
	return CrateData[Tag];
}

const FVesselUnitInfo& UDatabase::GetVesselUnitData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !VesselUnitData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting vessel unit %s"), *Tag.ToString());
	}
	return VesselUnitData[Tag];
}

		/* ITEM */

const FItemInfo& UDatabase::GetItemData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ItemData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting item %s"), *Tag.ToString());
	}
	return ItemData[Tag];
}

const FVesselItemInfo& UDatabase::GetVesselItemData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !VesselItemData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting vessel item %s"), *Tag.ToString());
	}
	return VesselItemData[Tag];
}

const FBuildableInfo& UDatabase::GetBuildableData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !BuildableData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting buildable item %s"), *Tag.ToString());
	}
	return BuildableData[Tag];
}

const FFoodInfo& UDatabase::GetFoodData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !FoodData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting food %s"), *Tag.ToString());
	}
	return FoodData[Tag];
}

const FClothesInfo& UDatabase::GetClothesData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ClothesData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting clothes %s"), *Tag.ToString());
	}
	return ClothesData[Tag];
}

const FRangedWeaponInfo& UDatabase::GetRangedWeaponData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !RangedWeaponData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting ranged weapon %s"), *Tag.ToString());
	}
	return RangedWeaponData[Tag];
}

const FAmmunitionInfo& UDatabase::GetAmmunitionData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !AmmunitionData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about unexisting ammunition %s"), *Tag.ToString());
	}
	return AmmunitionData[Tag];
}
