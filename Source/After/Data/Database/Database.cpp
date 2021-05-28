    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Database.cpp                //
////////////////////////////////////////

#include "Database.h"

#include "LogDatabase.h"
#include "../../Gameplay/Entity/Last.h"
#include "../../Gameplay/Entity/Mob/Mob.h"
#include "../../Gameplay/Entity/Mob/Animal.h"
//#include "../../Gameplay/Entity/Mob/Wolf.h"
#include "../../Gameplay/Entity/Mob/Mutant.h"
#include "../../Gameplay/Entity/Mob/Alien.h"
#include "../../Gameplay/Entity/Mob/Robot.h"
#include "../../Gameplay/Unit/Unit.h"
#include "../../Gameplay/Unit/Liquid/Liquid.h"
#include "../../Gameplay/Unit/SolidUnit/SolidUnit.h"
#include "../../Gameplay/Item/ThrownItem.h"
#include "../../Gameplay/Item/Item.h"

#include <functional>

template <typename T>
inline void InitField(TMap<FGameplayTag, T>& Map, const FString& TagName, const FString& DatabaseName, FDatabaseInitData& InitData,
					  const FExtraInfo& ExtraData, const std::function<void(const FGameplayTag&)>& Func = [](const FGameplayTag& i) { })
{
	TArray<FGameplayTag> Tags;

	Map.GetKeys(Tags);
	for (const FGameplayTag& i : Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogDatabase, Error, TEXT("There is an invalid tag %s in the %s database"), *i.ToString(), *DatabaseName);
		}
		if (!IS_TAG_PARENT(i, TagName))
		{
			UE_LOG(LogDatabase, Error, TEXT("%s is not %s, but it is contained in the %s database"), *i.ToString(), *DatabaseName, *DatabaseName);
		}

		Check(Map[i], i, InitData, ExtraData);

		Func(i);
	}
}

void UDatabase::Init()
{
	InitData.EntityReplaced.Empty();
	InitData.ItemReplaced.Empty();
	InitData.ProjectileReplaced.Empty();
	InitData.LiquidReplaced.Empty();
	InitData.SolidUnitReplaced.Empty();

	UE_LOG(LogDatabase, Log, TEXT("Database checking has been started"));

	// Check extra
	Check(ExtraData);

	// Check entity
	InitField<FEntityInfo>(EntityData, "entity", "entity", InitData, ExtraData,
		[&EntityData = EntityData, &EntityTagData = EntityTagData](const FGameplayTag& Tag)
	{
		EntityTagData.Empty();
		for (const FGameplayTag& i : EntityData[Tag].Tags)
		{
			if (!EntityTagData.Contains(i))
			{
				EntityTagData.Add(i);
			}
			EntityTagData[i].Objects.Add(Tag);
		}
	});

	// Check entity: last
	InitField<FLastInfo>(LastData, "entity.last", "last", InitData, ExtraData,
		[&LastData = LastData, &EntityData = EntityData](const FGameplayTag& Tag)
	{
		if (!EntityData.Contains(Tag))
		{
			UE_LOG(LogDatabase, Error, TEXT("The entity database does not contain a last %s, that is contained in the last database"), *Tag.ToString());
		}
		else if (EntityData[Tag].Class != LastData[Tag].Class)
		{
			EntityData[Tag].Class = LastData[Tag].Class;
			UE_LOG(LogDatabase, Warning, TEXT("Classes of a last %s are different in the last and entity databases (the class in the entity database has been changed)"), *Tag.ToString());
		}
	});

	// Check entity: mob
	InitField<FMobInfo>(MobData, "entity", "mob", InitData, ExtraData,
		[&MobData = MobData, &EntityData = EntityData](const FGameplayTag& Tag)
	{
		if (!EntityData.Contains(Tag))
		{
			UE_LOG(LogDatabase, Error, TEXT("The entity database does not contain a mob %s, that is contained in the mob database"), *Tag.ToString());
		}
		else if (EntityData[Tag].Class != MobData[Tag].Class)
		{
			EntityData[Tag].Class = MobData[Tag].Class;
			UE_LOG(LogDatabase, Warning, TEXT("Classes of a mob %s are different in the mob and entity databases (the class in the entity database has been changed)"), *Tag.ToString());
		}
	});

	// Check entity: animal
	InitField<FAnimalInfo>(AnimalData, "entity.animal", "animal", InitData, ExtraData,
		[&AnimalData = AnimalData, &MobData = MobData, &EntityData = EntityData](const FGameplayTag& Tag)
	{
		if (!MobData.Contains(Tag))
		{
			UE_LOG(LogDatabase, Error, TEXT("The mob database does not contain an animal %s, that is contained in the animal database"), *Tag.ToString());
		}
		else if (EntityData[Tag].Class != AnimalData[Tag].Class)
		{
			EntityData[Tag].Class = AnimalData[Tag].Class;
			MobData[Tag].Class = AnimalData[Tag].Class;
			UE_LOG(LogDatabase, Warning, TEXT("Classes of an animal %s are different in the animal and entity databases (the class in the entity database has been changed)"), *Tag.ToString());
		}
	});

	// Check entity: animal: wolf
	InitField<FWolfInfo>(WolfData, "entity.animal.wolf", "wolf", InitData, ExtraData,
		[&WolfData = WolfData, &AnimalData = AnimalData, &MobData = MobData, &EntityData = EntityData](const FGameplayTag& Tag)
	{
		if (!AnimalData.Contains(Tag))
		{
			UE_LOG(LogDatabase, Error, TEXT("The animal database does not contain a wolf %s, that is contained in the wolf database"), *Tag.ToString());
		}
/*		else if (EntityData[Tag].Class != WolfData[Tag].Class)
		{
			EntityData[Tag].Class = WolfData[Tag].Class;
			AnimalData[Tag].Class = WolfData[Tag].Class;
			MobData[Tag].Class = WolfData[Tag].Class;
			UE_LOG(LogDatabase, Warning, TEXT("Classes of a wolf %s are different in the wolf and entity databases (the class in the entity database has been changed)"), *Tag.ToString());
		}*/
	});

	// Check entity: mutant
	InitField<FMutantInfo>(MutantData, "entity.mutant", "mutant", InitData, ExtraData,
		[&MutantData = MutantData, &MobData = MobData, &EntityData = EntityData](const FGameplayTag& Tag)
	{
		if (!MobData.Contains(Tag))
		{
			UE_LOG(LogDatabase, Error, TEXT("The mob database does not contain a mutant %s, that is contained in the mutant database"), *Tag.ToString());
		}
		else if (EntityData[Tag].Class != MutantData[Tag].Class)
		{
			EntityData[Tag].Class = MutantData[Tag].Class;
			MobData[Tag].Class = MutantData[Tag].Class;
			UE_LOG(LogDatabase, Warning, TEXT("Classes of a mutant %s are different in the mutant and entity databases (the class in the entity database has been changed)"), *Tag.ToString());
		}
	});

	// Check entity: alien
	InitField<FAlienInfo>(AlienData, "entity.alien", "alien", InitData, ExtraData,
		[&AlienData = AlienData, &MobData = MobData, &EntityData = EntityData](const FGameplayTag& Tag)
	{
		if (!MobData.Contains(Tag))
		{
			UE_LOG(LogDatabase, Error, TEXT("The mob database does not contain an alien %s, that is contained in the alien database"), *Tag.ToString());
		}
		else if (EntityData[Tag].Class != AlienData[Tag].Class)
		{
			EntityData[Tag].Class = AlienData[Tag].Class;
			MobData[Tag].Class = AlienData[Tag].Class;
			UE_LOG(LogDatabase, Warning, TEXT("Classes of an alien %s are different in the alien and entity databases (the class in the entity database has been changed)"), *Tag.ToString());
		}
	});

	// Check entity: robot
	InitField<FRobotInfo>(RobotData, "entity.robot", "robot", InitData, ExtraData,
		[&RobotData = RobotData, &MobData = MobData, &EntityData = EntityData](const FGameplayTag& Tag)
	{
		if (!MobData.Contains(Tag))
		{
			UE_LOG(LogDatabase, Error, TEXT("The mob database does not contain a robot %s, that is contained in the robot database"), *Tag.ToString());
		}
		else if (EntityData[Tag].Class != RobotData[Tag].Class)
		{
			EntityData[Tag].Class = RobotData[Tag].Class;
			MobData[Tag].Class = RobotData[Tag].Class;
			UE_LOG(LogDatabase, Warning, TEXT("Classes of a robot %s are different in the robot and entity databases (the class in the entity database has been changed)"), *Tag.ToString());
		}
	});

	// Check unit
	InitField<FUnitInfo>(UnitData, "unit", "unit", InitData, ExtraData);

	// Check unit: liquid
	InitField<FLiquidInfo>(LiquidData, "unit.liquid", "liquid", InitData, ExtraData,
		[&LiquidData = LiquidData, &UnitData = UnitData, &LiquidTagData = LiquidTagData](const FGameplayTag& Tag)
	{
		LiquidTagData.Empty();
		for (const FGameplayTag& i : LiquidData[Tag].Tags)
		{
			if (!LiquidTagData.Contains(i))
			{
				LiquidTagData.Add(i);
			}
			LiquidTagData[i].Objects.Add(Tag);
		}

		if (!UnitData.Contains(Tag))
		{
			UE_LOG(LogDatabase, Error, TEXT("The unit database does not contain a liquid %s, that is contained in the liquid database"), *Tag.ToString());
		}
		else if (UnitData[Tag].Class != LiquidData[Tag].Class)
		{
			UnitData[Tag].Class = LiquidData[Tag].Class;
			UE_LOG(LogDatabase, Warning, TEXT("Classes of a liquid %s are different in the liquid and liquid databases (the class in the unit database has been changed)"), *Tag.ToString());
		}
	});

	// Check unit: solid unit
	InitField<FSolidUnitInfo>(SolidUnitData, "unit.solid", "solid unit", InitData, ExtraData,
		[&SolidUnitData = SolidUnitData, &UnitData = UnitData, &SolidUnitTagData = SolidUnitTagData](const FGameplayTag& Tag)
	{
		SolidUnitTagData.Empty();
		for (const FGameplayTag& i : SolidUnitData[Tag].Tags)
		{
			if (!SolidUnitTagData.Contains(i))
			{
				SolidUnitTagData.Add(i);
			}
			SolidUnitTagData[i].Objects.Add(Tag);
		}

		if (!UnitData.Contains(Tag))
		{
			UE_LOG(LogDatabase, Error, TEXT("The unit database does not contain a solid unit %s, that is contained in the solid unit database"), *Tag.ToString());
		}
		else if (UnitData[Tag].Class != SolidUnitData[Tag].Class)
		{
			UnitData[Tag].Class = SolidUnitData[Tag].Class;
			UE_LOG(LogDatabase, Warning, TEXT("Classes of a solid unit %s are different in the liquid and solid unit databases (the class in the unit database has been changed)"), *Tag.ToString());
		}
	});

	// Check unit: solid unit: desktop
	InitField<FDesktopInfo>(DesktopData, "unit.solid.desktop", "desktop", InitData, ExtraData);

	// Check unit: solid unit: crate
	InitField<FCrateInfo>(CrateData, "unit.solid.crate", "crate", InitData, ExtraData);

	// Check unit: solid unit: vessel
	InitField<FVesselUnitInfo>(VesselUnitData, "unit.solid.vessel", "vessel unit", InitData, ExtraData);

	// Check item
	InitField<FItemInfo>(ItemData, "item", "item", InitData, ExtraData,
		[&ItemData = ItemData, &ItemTagData = ItemTagData](const FGameplayTag& Tag)
	{
		ItemTagData.Empty();
		for (const FGameplayTag& i : ItemData[Tag].Tags)
		{
			if (!ItemTagData.Contains(i))
			{
				ItemTagData.Add(i);
			}
			ItemTagData[i].Objects.Add(Tag);
		}
	});

	// Check item: vessel item
	InitField<FVesselItemInfo>(VesselItemData, "item.vessel", "vessel item", InitData, ExtraData);

	// Check item: buildable
	InitField<FBuildableInfo>(BuildableData, "item.buildable", "buildable item", InitData, ExtraData);

	// Check item: food
	InitField<FFoodInfo>(FoodData, "item.food", "food", InitData, ExtraData);

	// Check item: clothes
	InitField<FClothesInfo>(ClothesData, "item.clothes", "clothes", InitData, ExtraData);

	// Check item: ranged weapon
	InitField<FRangedWeaponInfo>(RangedWeaponData, "item.weapon", "ranged weapon", InitData, ExtraData);

	// Check item: ammunition
	InitField<FAmmunitionInfo>(AmmunitionData, "item.ammunition", "ammunition", InitData, ExtraData,
		[&AmmunitionData = AmmunitionData, &AmmunitionTagData = AmmunitionTagData](const FGameplayTag& Tag)
	{
		AmmunitionTagData.Empty();
		for (const FGameplayTag& i : AmmunitionData[Tag].Tags)
		{
			if (!AmmunitionTagData.Contains(i))
			{
				AmmunitionTagData.Add(i);
			}
			AmmunitionTagData[i].Objects.Add(Tag);
		}
	});

	// Check projectile
	InitField<FProjectileInfo>(ProjectileData, "item.ammunition", "ammunition", InitData, ExtraData,
		[&ProjectileData = ProjectileData, &ProjectileTagData = ProjectileTagData](const FGameplayTag& Tag)
	{
		ProjectileTagData.Empty();
		for (const FGameplayTag& i : ProjectileData[Tag].Tags)
		{
			if (!ProjectileTagData.Contains(i))
			{
				ProjectileTagData.Add(i);
			}
			ProjectileTagData[i].Objects.Add(Tag);
		}
	});

	// Check profile: break
	InitField<FBreakProfileInfo>(BreakProfileData, "profile.break", "break profile", InitData, ExtraData);

	// Check profile: vessel
	InitField<FVesselProfileInfo>(VesselProfileData, "profile.vessel", "vessel profile", InitData, ExtraData);

	// Check profile: ammunition
	InitField<FAmmunitionProfileInfo>(AmmunitionProfileData, "profile.ammunition", "ammunition profile", InitData, ExtraData);

	// Check profile: behaviour
	InitField<FBehaviourProfileInfo>(BehaviourProfileData, "profile.behaviour", "behaviour profile", InitData, ExtraData);

	UE_LOG(LogDatabase, Log, TEXT("Database check has been finished"));
}

UDatabase::~UDatabase()
{
	Reset();
}

void UDatabase::Reset()
{
#if WITH_EDITOR
	UE_LOG(LogDatabase, Log, TEXT("Starting database reset"));
	for (const auto& i : InitData.EntityReplaced)
	{
		EntityData[i.Entity].Flipbooks[i.Status].Flipbooks[i.Direction] = nullptr;
	}
	for (const auto& i : InitData.ItemReplaced)
	{
		if (ItemData[i.Item].bUseFlipbook)
		{
			ItemData[i.Item].Flipbook = nullptr;
		}
		else
		{
			ItemData[i.Item].Sprite = nullptr;
		}
	}
	for (const auto& i : InitData.ProjectileReplaced)
	{
		ProjectileData[i.Projectile].Sprite = nullptr;
	}
	for (const auto& i : InitData.LiquidReplaced)
	{
		LiquidData[i.Liquid].Flipbooks[i.Status] = nullptr;
	}
	for (const auto& i : InitData.SolidUnitReplaced)
	{
		if (SolidUnitData[i.SolidUnit].bUseFlipbook)
		{
			SolidUnitData[i.SolidUnit].Flipbook = nullptr;
		}
		else
		{
			SolidUnitData[i.SolidUnit].Sprite = nullptr;
		}
	}

	InitData.EntityReplaced.Empty();
	InitData.ItemReplaced.Empty();
	InitData.ProjectileReplaced.Empty();
	InitData.LiquidReplaced.Empty();
	InitData.SolidUnitReplaced.Empty();

	UE_LOG(LogDatabase, Log, TEXT("Database reset has been finished"));
#endif // WITH_EDITOR
}

		/* ENTITY */

const FEntityInfo& UDatabase::GetEntityData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !EntityData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about entity %s that is not contained in the database"), *Tag.ToString());
	}
	return EntityData[Tag];
}

const FLastInfo& UDatabase::GetLastData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !LastData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about Last %s that is not contained in the database"), *Tag.ToString());
	}
	return LastData[Tag];
}

const FMobInfo& UDatabase::GetMobData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !MobData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about mob %s that is not contained in the database"), *Tag.ToString());
	}
	return MobData[Tag];
}

const FAnimalInfo& UDatabase::GetAnimalData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !AnimalData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about animal %s that is not contained in the database"), *Tag.ToString());
	}
	return AnimalData[Tag];
}

const FWolfInfo& UDatabase::GetWolfData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !WolfData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about wolf %s that is not contained in the database"), *Tag.ToString());
	}
	return WolfData[Tag];
}

const FMutantInfo& UDatabase::GetMutantData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !MutantData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about mutant %s that is not contained in the database"), *Tag.ToString());
	}
	return MutantData[Tag];
}

const FAlienInfo& UDatabase::GetAlienData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !AlienData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about alien %s that is not contained in the database"), *Tag.ToString());
	}
	return AlienData[Tag];
}

const FRobotInfo& UDatabase::GetRobotData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !RobotData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about robot %s that is not contained in the database"), *Tag.ToString());
	}
	return RobotData[Tag];
}

		/* UNIT */

const FUnitInfo& UDatabase::GetUnitData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !UnitData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about unit %s that is not contained in the database"), *Tag.ToString());
	}
	return UnitData[Tag];
}

const FLiquidInfo& UDatabase::GetLiquidData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !LiquidData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about liquid %s that is not contained in the database"), *Tag.ToString());
	}
	return LiquidData[Tag];
}

const FSolidUnitInfo& UDatabase::GetSolidUnitData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !SolidUnitData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about solid unit %s that is not contained in the database"), *Tag.ToString());
	}
	return SolidUnitData[Tag];
}

const FDesktopInfo& UDatabase::GetDesktopData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !DesktopData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about desktop %s that is not contained in the database"), *Tag.ToString());
	}
	return DesktopData[Tag];
}

const FCrateInfo& UDatabase::GetCrateData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !CrateData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about crate %s that is not contained in the database"), *Tag.ToString());
	}
	return CrateData[Tag];
}

const FVesselUnitInfo& UDatabase::GetVesselUnitData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !VesselUnitData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about vessel unit %s that is not contained in the database"), *Tag.ToString());
	}
	return VesselUnitData[Tag];
}

		/* ITEM */

const FItemInfo& UDatabase::GetItemData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ItemData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about item %s that is not contained in the database"), *Tag.ToString());
	}
	return ItemData[Tag];
}

const FVesselItemInfo& UDatabase::GetVesselItemData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !VesselItemData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about vessel item %s that is not contained in the database"), *Tag.ToString());
	}
	return VesselItemData[Tag];
}

const FBuildableInfo& UDatabase::GetBuildableData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !BuildableData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about buildable item %s that is not contained in the database"), *Tag.ToString());
	}
	return BuildableData[Tag];
}

const FFoodInfo& UDatabase::GetFoodData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !FoodData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about food %s that is not contained in the database"), *Tag.ToString());
	}
	return FoodData[Tag];
}

const FClothesInfo& UDatabase::GetClothesData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ClothesData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about clothes %s that is not contained in the database"), *Tag.ToString());
	}
	return ClothesData[Tag];
}

const FRangedWeaponInfo& UDatabase::GetRangedWeaponData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !RangedWeaponData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about ranged weapon %s that is not contained in the database"), *Tag.ToString());
	}
	return RangedWeaponData[Tag];
}

const FAmmunitionInfo& UDatabase::GetAmmunitionData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !AmmunitionData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about ammunition %s that is not contained in the database"), *Tag.ToString());
	}
	return AmmunitionData[Tag];
}

		/* PROJECTILE */

const FProjectileInfo& UDatabase::GetProjectileData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ProjectileData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about projectile %s that is not contained in the database"), *Tag.ToString());
	}
	return ProjectileData[Tag];
}

		/* TAG */

const FTagInfo& UDatabase::GetEntityTagData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !EntityTagData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about entity tag %s that is not contained in the database"), *Tag.ToString());
	}
	return EntityTagData[Tag];
}

const FTagInfo& UDatabase::GetLiquidTagData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !LiquidTagData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about liquid tag %s that is not contained in the database"), *Tag.ToString());
	}
	return LiquidTagData[Tag];
}

const FTagInfo& UDatabase::GetSolidUnitTagData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !SolidUnitTagData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about solid unit tag %s that is not contained in the database"), *Tag.ToString());
	}
	return SolidUnitTagData[Tag];
}

const FTagInfo& UDatabase::GetItemTagData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ItemTagData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about item tag %s that is not contained in the database"), *Tag.ToString());
	}
	return ItemTagData[Tag];
}

const FTagInfo& UDatabase::GetAmmunitionTagData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ItemTagData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about ammunition tag %s that is not contained in the database"), *Tag.ToString());
	}
	return ItemTagData[Tag];
}

const FTagInfo& UDatabase::GetProjectileTagData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ProjectileTagData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about projectile tag %s that is not contained in the database"), *Tag.ToString());
	}
	return ProjectileTagData[Tag];
}

		/* PROFILE */

const FBreakProfileInfo& UDatabase::GetBreakProfileData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !BreakProfileData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about break profile %s that is not contained in the database"), *Tag.ToString());
	}
	return BreakProfileData[Tag];
}

const FVesselProfileInfo& UDatabase::GetVesselProfileData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !VesselProfileData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about vessel profile %s that is not contained in the database"), *Tag.ToString());
	}
	return VesselProfileData[Tag];
}

const FAmmunitionProfileInfo& UDatabase::GetAmmunitionProfileData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !AmmunitionProfileData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about ammunition profile %s that is not contained in the database"), *Tag.ToString());
	}
	return AmmunitionProfileData[Tag];
}

const FBehaviourProfileInfo& UDatabase::GetBehaviourProfileData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !BehaviourProfileData.Contains(Tag))
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Attempt to get data about behaviour profile %s that is not contained in the database"), *Tag.ToString());
	}
	return BehaviourProfileData[Tag];
}

		/* EXTRA */

const FExtraInfo& UDatabase::GetExtraData() const
{
	return ExtraData;
}