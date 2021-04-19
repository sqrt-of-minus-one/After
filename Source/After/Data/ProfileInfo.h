    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: ProfileInfo.h               //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Types.h"

#include "ProfileInfo.generated.h"

USTRUCT(BlueprintType)
struct FBreakeProfileGroup
{
	GENERATED_USTRUCT_BODY()

public:
	// Tags that item must have to be able to breake the unit
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

USTRUCT(BlueprintType)
struct FVesselProfileInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// If liquid have one of these tags, it can be contained by the vessel
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> CanStand;
};

USTRUCT(BlueprintType)
struct FAmmunitionProfileInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// If ammunition have one of these tags, it can be used with the weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> CanUse;
};
