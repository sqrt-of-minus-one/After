    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: EntityInfo.h                //
////////////////////////////////////////

#include "EntityInfo.h"

void Check(const FEntityInfo& Data, const FGameplayTag& Tag)
{
	// General
	if (Data.Name.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Entity %s doesn't have a name"), *Tag.ToString());
	}
	if (Data.Description.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Entity %s doesn't have a description"), *Tag.ToString());
	}
	for (const FGameplayTag& i : Data.Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Entity %s contains an invalid entity tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.entity"))
		{
			UE_LOG(LogTemp, Error, TEXT("Entity %s contains a tag with invalid name (%s is not an entity tag)"), *Tag.ToString(), *i.ToString());
		}
	}

	// Stats
	if (Data.MaxHealth <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Entity %s has non-positive maximum health (%f)"), *Tag.ToString(), Data.MaxHealth);
	}
	if (Data.MaxOxygen <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Entity %s has non-positive maximum oxygen (%f)"), *Tag.ToString(), Data.MaxOxygen);
	}
	if (Data.MaxEnergy <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Entity %s has non-positive maximum energy (%f)"), *Tag.ToString(), Data.MaxEnergy);
	}
	if (Data.EnergySpeed <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Entity %s has non-positive energy decreasing speed (%f)"), *Tag.ToString(), Data.EnergySpeed);
	}
	if (Data.EnergyRegenerationSpeed <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Entity %s has non-positive energy regeneration speed (%f)"), *Tag.ToString(), Data.EnergyRegenerationSpeed);
	}

	// Movement
	if (Data.WalkSpeed < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Entity %s has negative walking speed (%f)"), *Tag.ToString(), Data.WalkSpeed);
	}
	if (Data.RunSpeed < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Entity %s has negative running speed (%f)"), *Tag.ToString(), Data.RunSpeed);
	}
	if (Data.SwimSpeed < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Entity %s has negative swimming speed (%f)"), *Tag.ToString(), Data.SwimSpeed);
	}

	// Damage
	for_enum<FDamageType>([&Data, &Tag](FDamageType i, bool& out_continue)
	{
		if (!Data.DamageResist.Contains(i))
		{
			UE_LOG(LogTemp, Error, TEXT("Entity %s doesn't have one of damage resist values (#%d)"), *Tag.ToString(), i);
		}
		else if (Data.DamageResist[i] == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("One of damage resist values (#%d) of entity %s is zero (may cause divide by zero)"), i, *Tag.ToString());
		}
	});

	// Attack
	if (Data.AttackRadius < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Entity %s has negative attack radius (%f)"), *Tag.ToString(), Data.AttackRadius);
	}
	if (Data.RangedProjectile != FGameplayTag::EmptyTag)
	{
		if (!Data.RangedProjectile.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Entity %s has an invalid projectile (%s)"), *Tag.ToString(), *Data.RangedProjectile.ToString());
		}
		if (!IS_TAG_PARENT(Data.RangedProjectile, "projectile"))
		{
			UE_LOG(LogTemp, Error, TEXT("Entity %s has a projectile with invalid name (%s is not a projectile)"), *Tag.ToString(), *Data.RangedProjectile.ToString());
		}
		if (Data.RangedRadiusMultiplier <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Entity %s has non-positive ranged radius multiplier (%f)"), *Tag.ToString(), Data.RangedRadiusMultiplier);
		}
		if (Data.RangedSpeedMultiplier <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Entity %s has non-positive ranged speed multiplier (%f)"), *Tag.ToString(), Data.RangedSpeedMultiplier);
		}
		if (Data.RangedDamageMultiplier <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Entity %s has non-positive ranged damage multiplier (%f)"), *Tag.ToString(), Data.RangedDamageMultiplier);
		}
		if (Data.FiringRate <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Entity %s has non-positive firing rate (%f)"), *Tag.ToString(), Data.FiringRate);
		}
	}

	// Appearance
	for_enum<FEntityStatus>([&Data, &Tag](FEntityStatus i, bool& out_continue_i)
	{
		if (!Data.Flipbooks.Contains(i))
		{
			UE_LOG(LogTemp, Error, TEXT("There are flipbooks that entity %s doesn't have"), *Tag.ToString());
			out_continue_i = false;
		}
		else for_enum<FDirection>([&Data, i, &out_continue_i, &Tag](FDirection j, bool& out_continue_j)
		{
			if (!Data.Flipbooks[i].Flipbooks.Contains(j) || !Data.Flipbooks[i].Flipbooks[j])
			{
				UE_LOG(LogTemp, Error, TEXT("There are flipbooks that entity %s doesn't have"), *Tag.ToString());
				out_continue_j = false;
				out_continue_i = false;
			}
		});
	});

	// Audio
	for_enum<FEntitySoundType>([&Data, &Tag](FEntitySoundType i, bool& out_continue)
	{
		if (!Data.Sounds.Sounds.Contains(i))
		{
			UE_LOG(LogTemp, Error, TEXT("There are sound types that entity %s doesn't have (they should be added but may be empty)"), *Tag.ToString());
			out_continue = false;
		}
	});
}

void Check(const FLastInfo& Data, const FGameplayTag& Tag)
{
	// Stats
	if (Data.MaxSatiety <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Last %s has non-positive maximum satiety (%f)"), *Tag.ToString(), Data.MaxSatiety);
	}
	if (Data.SatietySpeed <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Lasr %s has non-positive satiety decreasing speed (%f)"), *Tag.ToString(), Data.SatietySpeed);
	}
}

void Check(const FMobInfo& Data, const FGameplayTag& Tag)
{
	//Drop
	for (const FItemDrop& i : Data.Drop)
	{
		if (!i.Item.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Mob %s has an invalid drop (%s)"), *Tag.ToString(), *i.Item.ToString());
		}
		if (!IS_TAG_PARENT(i.Item, "item"))
		{
			UE_LOG(LogTemp, Error, TEXT("Mob %s has a drop with invalid name (%s is not an item)"), *Tag.ToString(), *i.Item.ToString());
		}
		if (i.Min < 0 || i.Max < i.Min)
		{
			UE_LOG(LogTemp, Error, TEXT("Mob %s has wrong minimum and maximum amount of drop %s (min = %d; max = %d)"), *Tag.ToString(), *i.Item.ToString(), i.Min, i.Max);
		}
		if (i.Chance < 0 || i.Chance > 1)
		{
			UE_LOG(LogTemp, Error, TEXT("Mob %s has wrong chance of drop %s (%f). It must be between 0 and 1"), *Tag.ToString(), *i.Item.ToString(), i.Chance);
		}
	}
	if (Data.MinExperience < 0 || Data.MaxExperience < Data.MinExperience)
	{
		UE_LOG(LogTemp, Error, TEXT("Mob %s has wrong minimum and maximum amount of experience (min = %f; max = %f)"), *Tag.ToString(), Data.MinExperience, Data.MaxExperience);
	}
	if (Data.ViewRadius <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Mob %s has non-positive value of view radius (%f)"), *Tag.ToString(), Data.ViewRadius);
	}
	if (Data.PursueRadius <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Mob %s has non-positive value of pursue radius (%f)"), *Tag.ToString(), Data.PursueRadius);
	}
	
	// Behaviour
	if (!Data.BehaviourProfile.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Mob %s has an invalid behaviour profile (%s)"), *Tag.ToString(), *Data.BehaviourProfile.ToString());
	}
	if (!IS_TAG_PARENT(Data.BehaviourProfile, "profile.behaviour"))
	{
		UE_LOG(LogTemp, Error, TEXT("Mob %s has a behaviour profile with invalid name (%s is not a behaviour profile)"), *Tag.ToString(), *Data.BehaviourProfile.ToString());
	}
}

void Check(const FAnimalInfo& Data, const FGameplayTag& Tag)
{
	// Mutation
	if (!Data.Mutant.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Animal %s has an invalid mutated analog (%s)"), *Tag.ToString(), *Data.Mutant.ToString());
	}
	if (!IS_TAG_PARENT(Data.Mutant, "entity.mutant"))
	{
		UE_LOG(LogTemp, Error, TEXT("Animal %s has a mutated analog with invalid name (%s is not a mutant)"), *Tag.ToString(), *Data.Mutant.ToString());
	}
}

void Check(const FWolfInfo& Data, const FGameplayTag& Tag)
{
	// Stats
	if (Data.MaxSatiety <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Wolf %s has non-positive maximum satiety (%f)"), *Tag.ToString(), Data.MaxSatiety);
	}
	if (Data.SatietySpeed <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Wolf %s has non-positive satiety decreasing speed (%f)"), *Tag.ToString(), Data.SatietySpeed);
	}
}

void Check(const FMutantInfo& Data, const FGameplayTag& Tag)
{
}

void Check(const FAlienInfo& Data, const FGameplayTag& Tag)
{
}

void Check(const FRobotInfo& Data, const FGameplayTag& Tag)
{
}
