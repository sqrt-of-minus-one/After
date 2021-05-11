    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: ProjectileInfo.cpp          //
////////////////////////////////////////

#include "ProjectileInfo.h"

#include "DatabaseInitData.h"
#include "ExtraInfo.h"
#include "LogDatabase.h"

void Check(FProjectileInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	// General
	for (const FGameplayTag& i : Data.Tags)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogDatabase, Fatal, TEXT("Projectile %s contains an invalid projectile tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.projectile"))
		{
			UE_LOG(LogDatabase, Fatal, TEXT("Projectile %s contains a tag with invalid name (%s is not an projectile tag)"), *Tag.ToString(), *i.ToString());
		}
	}

	// Characteristics
	if (Data.Distance <= 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Projectile %s has non-positive distance (%f)"), *Tag.ToString(), Data.Distance);
	}
	if (Data.Push < 0)
	{
		UE_LOG(LogDatabase, Error, TEXT("Projectile %s has negative push (%f)"), *Tag.ToString(), Data.Push);
	}

	// Appearance
	if (!Data.Sprite)
	{
		UE_LOG(LogDatabase, Error, TEXT("Projectile %s doesn't have sprite"), *Tag.ToString());
		InitData.ProjectileReplaced.AddTail({ Tag });
		Data.Sprite = ExtraData.DebugProjectileSprite;
	}
}
