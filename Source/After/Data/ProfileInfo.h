    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: ProfileInfo.h               //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Types.h"

#include "ProfileInfo.generated.h"

struct FDatabaseInitData;
struct FExtraInfo;

USTRUCT(BlueprintType)
struct FBreakeProfileGroup
{
	GENERATED_USTRUCT_BODY()

public:
	// Tags that item must have to be able to breake the unit <<tag.item || item>>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> Group;
};

USTRUCT(BlueprintType)
struct FBreakeProfileInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// Whether the unit can be broken without special tools
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeBrokenByHand;

	// Tags that item must have to be able to breake the unit. That's enough to have only tags from one of the groups
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FBreakeProfileGroup> CanBeBrokenBy;
};
void Check(FBreakeProfileInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FVesselProfileInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// If liquid have one of these tags, it can be contained by the vessel <<tag.liquid || unit.liquid>>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> CanStand;
};
void Check(FVesselProfileInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FAmmunitionProfileInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// If ammunition have one of these tags, it can be used with the weapon <<tag.ammunition || item.ammunition>>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> CanUse;
};
void Check(FAmmunitionProfileInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);

USTRUCT(BlueprintType)
struct FBehaviourProfileInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// Whether entity attacks everyone
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsAgressive;

	// Exceptions to is agressive <<tag.entity || entity>>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> AgressiveExcept;
	
	// Whether entity runs away from everyone
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsFearful;

	// Exceptions to is fearful <<tag.entity || entity>>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> FearfulExcept;
};
void Check(FBehaviourProfileInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData);
