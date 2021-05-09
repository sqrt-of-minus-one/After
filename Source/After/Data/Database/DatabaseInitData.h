    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: DatabaseInitData.h          //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "../Types.h"

struct FDatabaseInitData
{
	struct FEntityReplaced
	{
		FGameplayTag Entity;
		FEntityStatus Status;
		FDirection Direction;
	};
	struct FItemReplaced
	{
		FGameplayTag Item;
	};
	struct FProjectileReplaced
	{
		FGameplayTag Projectile;
	};
	struct FLiquidReplaced
	{
		FGameplayTag Liquid;
		FLiquidStatus Status;
	};
	struct FSolidUnitReplaced
	{
		FGameplayTag SolidUnit;
	};

public:
	TDoubleLinkedList<FEntityReplaced> EntityReplaced;
	TDoubleLinkedList<FItemReplaced> ItemReplaced;
	TDoubleLinkedList<FProjectileReplaced> ProjectileReplaced;
	TDoubleLinkedList<FLiquidReplaced> LiquidReplaced;
	TDoubleLinkedList<FSolidUnitReplaced> SolidUnitReplaced;
};
