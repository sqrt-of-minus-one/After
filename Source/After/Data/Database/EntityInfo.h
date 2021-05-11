    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: EntityInfo.h                //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "../Types.h"

#include "EntityInfo.generated.h"

struct FDatabaseInitData;
struct FExtraInfo;
class AEntity;
class ALast;
class AMob;
class AAnimal;
class AMutant;
class AAlien;
class ARobot;

USTRUCT(BlueprintType)
struct FEntityInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Entity's class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSubclassOf<AEntity> Class;

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

	// How hard the entity pushes its target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float Push;

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

	// The entity's size
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FIntPoint Size;

			/* AUDIO */

	// The entity's sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	FEntitySounds Sounds;

	// Minimum pause between entity's sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	float MinSoundPause;

	// Maximum pause between entity's sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	float MaxSoundPause;
};
void Check(FEntityInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FLastInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Last's class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSubclassOf<ALast> Class;

			/* STATS */

	// Maximum value of the satiety
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MaxSatiety;

	// Satiety decreasing per (tick * delta)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float SatietySpeed;
};
void Check(FLastInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FMobInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Mob's class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSubclassOf<AMob> Class;

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
void Check(FMobInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FAnimalInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Animal's class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSubclassOf<AAnimal> Class;

			/* MUTATION */

	// Mutant kind that the animal can turn into (<<entity.mutant>>)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mutation")
	FGameplayTag Mutant;
};
void Check(FAnimalInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

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
void Check(FWolfInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FMutantInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Mutant's class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSubclassOf<AMutant> Class;
};
void Check(FMutantInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FAlienInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Alien's class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSubclassOf<AAlien> Class;
};
void Check(FAlienInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FRobotInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// Robot's class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TSubclassOf<ARobot> Class;
};
void Check(FRobotInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);
