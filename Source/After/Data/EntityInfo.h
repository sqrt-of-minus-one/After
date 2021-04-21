    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: EntityInfo.h                //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Types.h"

#include "EntityInfo.generated.h"

USTRUCT(BlueprintType)
struct FEntityInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// User-friendly entity's name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FText Name;

	// Entity's description
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FText Description;

	// Entity's tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TArray<FGameplayTag> Tags;

			/* STATS */

	// Maximum value of the entity's health
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MaxHealth;

	// Maximum value of the entity's oxygen
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MaxOxygen;

	// Maximum value of the entity's energy
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MaxEnergy;

	// Oxygen decreasing per (tick * delta) while the entity is underwater
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float OxygenSpeed;

	// Oxygen increasing per (tick * delta) while the entity is not underwater
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float OxygenRegenerationSpeed;

	// Energy decreasing per (tick * delta) while the entity is running
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float EnergySpeed;

	// Energy increasing per (tick * delta) while the entity is not running
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float EnergyRegenerationSpeed;

			/* MOVEMENT */

	// Walking speed (tick * delta)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed;

	// Running speed (tick * delta)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float RunSpeed;

	// Swimming speed (tick * delta)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SwimSpeed;

	// Whether entity need stairs to move between layers
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bNeedStairs = true;

			/* DAMAGE */

	// Values by which damage is divided (more than 1 = resist; less than 1 = vulnerability)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FDamageType, float> DamageResist;

			/* ATTACK */

	// Melee attack damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float Damage;

	// Melee attack damage type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	FDamageType DamageType;

	// Melee attack radius
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRadius;

	// Projectile that is used in ranged attack (<<projectile>>)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	FGameplayTag RangedProjectile;

	// The projectile's range is multiplied by this value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float RangedRadiusMultiplier = 1.f;

	// The projectile's speed is multiplied by this value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float RangedSpeedMultiplier = 1.f;

	// The projectile's damage is multiplied by this value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float RangedDamageMultiplier = 1.f;

	// Projectiles per second
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float FiringRate;

			/* INTERACTION */

	// Whether entity can be selected by player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	bool bSelectable = true;

			/* APPEARANCE */

	// The entity's flipbooks
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TMap<FEntityStatus, FEntityFlipbooks> Flipbooks;

			/* AUDIO */

	// The entity's sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	FEntitySounds Sounds;
};
void Check(const FEntityInfo& Data, const FGameplayTag& Tag);

USTRUCT(BlueprintType)
struct FLastInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* STATS */

	// Maximum value of the satiety
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MaxSatiety;

	// Satiety decreasing per (tick * delta)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float SatietySpeed;
};
void Check(const FLastInfo& Data, const FGameplayTag& Tag);

USTRUCT(BlueprintType)
struct FMobInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* DROP */

	// Items that may be dropped
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drop")
	TArray<FItemDrop> Drop;

	// Minimum amount of experience
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drop")
	float MinExperience;

	// Maximum amount of experience
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drop")
	float MaxExperience;

			/* VIEW */

	// The distance at which the entity can see
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "View")
	float ViewRadius;

	// The distance at which the entity can see its target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "View")
	float PursueRadius;

			/* BEHAVIOUR */

	// The behaviour towards other entities (<<profile.agressive>>)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behaviour")
	FGameplayTag BehaviourProfile;
};
void Check(const FMobInfo& Data, const FGameplayTag& Tag);

USTRUCT(BlueprintType)
struct FAnimalInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* MUTATION */

	// Mutant kind that the animal can turn into (<<entity.mutant>>)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mutation")
	FGameplayTag Mutant;
};
void Check(const FAnimalInfo& Data, const FGameplayTag& Tag);

USTRUCT(BlueprintType)
struct FWolfInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* STATS */

	// Maximum value of the satiety
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MaxSatiety;

	// Satiety decreasing per (tick * delta)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float SatietySpeed;
};
void Check(const FWolfInfo& Data, const FGameplayTag& Tag);

USTRUCT(BlueprintType)
struct FMutantInfo
{
	GENERATED_USTRUCT_BODY()

public:

};
void Check(const FMutantInfo& Data, const FGameplayTag& Tag);

USTRUCT(BlueprintType)
struct FAlienInfo
{
	GENERATED_USTRUCT_BODY()

public:

};
void Check(const FAlienInfo& Data, const FGameplayTag& Tag);

USTRUCT(BlueprintType)
struct FRobotInfo
{
	GENERATED_USTRUCT_BODY()

public:

};
void Check(const FRobotInfo& Data, const FGameplayTag& Tag);
