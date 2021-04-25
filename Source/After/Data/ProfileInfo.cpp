    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: ProfileInfo.cpp             //
////////////////////////////////////////

#include "ProfileInfo.h"

#include "DatabaseInitData.h"
#include "ExtraInfo.h"

void Check(FBreakeProfileInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	for (const FBreakeProfileGroup& i : Data.CanBeBrokenBy)
	{
		for (const FGameplayTag& j : i.Group)
		{
			if (!j.IsValid())
			{
				UE_LOG(LogTemp, Fatal, TEXT("Breake profile %s contains an invalid tag (%s)"), *Tag.ToString(), *j.ToString());
			}
			if (!IS_TAG_PARENT(j, "tag.item") && !IS_TAG_PARENT(j, "item"))
			{
				UE_LOG(LogTemp, Fatal, TEXT("Breake profile %s contains a tag with invalid name (%s is not an item or an item tag)"), *Tag.ToString(), *j.ToString());
			}
		}
	}
}

void Check(FVesselProfileInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	for (const FGameplayTag& i : Data.CanStand)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Fatal, TEXT("Vessel profile %s contains an invalid tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.liquid") && !IS_TAG_PARENT(i, "unit.liquid"))
		{
			UE_LOG(LogTemp, Fatal, TEXT("Vessel profile %s contains a tag with invalid name (%s is not a liquid or a liquid tag)"), *Tag.ToString(), *i.ToString());
		}
	}
}

void Check(FAmmunitionProfileInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	for (const FGameplayTag& i : Data.CanUse)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Fatal, TEXT("Ammunition profile %s contains an invalid tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.ammunition") && !IS_TAG_PARENT(i, "item.ammunition"))
		{
			UE_LOG(LogTemp, Fatal, TEXT("Ammunition profile %s contains a tag with invalid name (%s is not an ammunition or an ammunition tag)"), *Tag.ToString(), *i.ToString());
		}
	}
}

void Check(FBehaviourProfileInfo& Data, const FGameplayTag& Tag, FDatabaseInitData& InitData, const FExtraInfo& ExtraData)
{
	for (const FGameplayTag& i : Data.Except)
	{
		if (!i.IsValid())
		{
			UE_LOG(LogTemp, Fatal, TEXT("Behaviour profile %s contains an invalid tag (%s)"), *Tag.ToString(), *i.ToString());
		}
		if (!IS_TAG_PARENT(i, "tag.entity") && !IS_TAG_PARENT(i, "entity"))
		{
			UE_LOG(LogTemp, Fatal, TEXT("Behaviour profile %s contains a tag with invalid name (%s is not an entity or an entity tag)"), *Tag.ToString(), *i.ToString());
		}
	}
}
