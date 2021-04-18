    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: ProjectileInfo.h            //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Types.h"

#include "ProjectileInfo.generated.h"

class UPaperSprite;

USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* GENERAL */

	// User-friendly projectile's name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FText Name;

	// Projectile's description
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FText Description;

	// Projectile's tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TArray<FGameplayTag> Tags;

			/* CHARACTERISTICS */

	// Damage that is dealt by the projectile
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	float Damage;

	// Projectile's damage type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	FDamageType DamageType;

	// Maximum projectile's range
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Characteristics")
	float Distance;

			/* APPEARANCE */

	// Projectile's sprite
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	UPaperSprite* Sprite;
};