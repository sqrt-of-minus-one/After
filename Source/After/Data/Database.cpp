    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Database.cpp                //
////////////////////////////////////////

#include "Database.h"

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
			UE_LOG(LogTemp, Error, TEXT("There is an invalid tag %s in the %s database"), *i.ToString(), *DatabaseName);
		}
		if (!IS_TAG_PARENT(i, TagName))
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not %s, but it is contained in the %s database"), *i.ToString(), *DatabaseName, *DatabaseName);
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
	InitField<FLastInfo>(LastData, "entity.last", "last", InitData, ExtraData);

	// Check entity: mob
	InitField<FMobInfo>(MobData, "entity", "mob", InitData, ExtraData);

	// Check entity: animal
	InitField<FAnimalInfo>(AnimalData, "entity.animal", "animal", InitData, ExtraData);

	// Check entity: animal: wolf
	InitField<FWolfInfo>(WolfData, "entity.animal.wolf", "wolf", InitData, ExtraData);

	// Check entity: mutant
	InitField<FMutantInfo>(MutantData, "entity.mutant", "mutant", InitData, ExtraData);

	// Check entity: alien
	InitField<FAlienInfo>(AlienData, "entity.alien", "alien", InitData, ExtraData);

	// Check entity: robot
	InitField<FRobotInfo>(RobotData, "entity.robot", "robot", InitData, ExtraData);

	// Check unit
	InitField<FUnitInfo>(UnitData, "unit", "unit", InitData, ExtraData);

	// Check unit: liquid
	InitField<FLiquidInfo>(LiquidData, "unit.liquid", "liquid", InitData, ExtraData,
		[&LiquidData = LiquidData, &LiquidTagData = LiquidTagData](const FGameplayTag& Tag)
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
	});

	// Check unit: solid unit
	InitField<FSolidUnitInfo>(SolidUnitData, "unit.solid", "solid unit", InitData, ExtraData,
		[&SolidUnitData = SolidUnitData, &SolidUnitTagData = SolidUnitTagData](const FGameplayTag& Tag)
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

	// Check profile: breake
	InitField<FBreakeProfileInfo>(BreakeProfileData, "profile.breake", "breake profile", InitData, ExtraData);

	// Check profile: vessel
	InitField<FVesselProfileInfo>(VesselProfileData, "profile.vessel", "vessel profile", InitData, ExtraData);

	// Check profile: ammunition
	InitField<FAmmunitionProfileInfo>(AmmunitionProfileData, "profile.ammunition", "ammunition profile", InitData, ExtraData);

	// Check profile: behaviour
	InitField<FBehaviourProfileInfo>(BehaviourProfileData, "profile.behaviour", "behaviour profile", InitData, ExtraData);
}

UDatabase::~UDatabase()
{
	Reset();
}

void UDatabase::Reset()
{
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

		/* PROJECTILE */

const FProjectileInfo& UDatabase::GetProjectileData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ProjectileData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about projectile %s that is not contained in the database"), *Tag.ToString());
	}
	return ProjectileData[Tag];
}

		/* TAG */

const FTagInfo& UDatabase::GetEntityTagData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !EntityTagData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about entity tag %s that is not contained in the database"), *Tag.ToString());
	}
	return EntityTagData[Tag];
}

const FTagInfo& UDatabase::GetLiquidTagData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !LiquidTagData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about liquid tag %s that is not contained in the database"), *Tag.ToString());
	}
	return LiquidTagData[Tag];
}

const FTagInfo& UDatabase::GetSolidUnitTagData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !SolidUnitTagData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about solid unit tag %s that is not contained in the database"), *Tag.ToString());
	}
	return SolidUnitTagData[Tag];
}

const FTagInfo& UDatabase::GetItemTagData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ItemTagData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about item tag %s that is not contained in the database"), *Tag.ToString());
	}
	return ItemTagData[Tag];
}

const FTagInfo& UDatabase::GetAmmunitionTagData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ItemTagData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about ammunition tag %s that is not contained in the database"), *Tag.ToString());
	}
	return ItemTagData[Tag];
}

const FTagInfo& UDatabase::GetProjectileTagData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !ProjectileTagData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about projectile tag %s that is not contained in the database"), *Tag.ToString());
	}
	return ProjectileTagData[Tag];
}

		/* PROFILE */

const FBreakeProfileInfo& UDatabase::GetBreakeProfileData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !BreakeProfileData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about breake profile %s that is not contained in the database"), *Tag.ToString());
	}
	return BreakeProfileData[Tag];
}

const FVesselProfileInfo& UDatabase::GetVesselProfileData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !VesselProfileData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about vessel profile %s that is not contained in the database"), *Tag.ToString());
	}
	return VesselProfileData[Tag];
}

const FAmmunitionProfileInfo& UDatabase::GetAmmunitionProfileData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !AmmunitionProfileData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about ammunition profile %s that is not contained in the database"), *Tag.ToString());
	}
	return AmmunitionProfileData[Tag];
}

const FBehaviourProfileInfo& UDatabase::GetBehaviourProfileData(const FGameplayTag Tag) const
{
	if (!Tag.IsValid() || !BehaviourProfileData.Contains(Tag))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Attempt to get data about behaviour profile %s that is not contained in the database"), *Tag.ToString());
	}
	return BehaviourProfileData[Tag];
}

		/* EXTRA */

const FExtraInfo& UDatabase::GetExtraData() const
{
	return ExtraData;
}