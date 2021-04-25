    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: ExtraInfo.cpp               //
////////////////////////////////////////

#include "ExtraInfo.h"

void Check(FExtraInfo& Data)
{
	for_enum<FDirection>([&Data](FDirection i, bool& out_continue)
	{
		if (!Data.DebugEntityFlipbooks.Contains(i))
		{
			Data.DebugEntityFlipbooks.Add(i);
		}
	});
	for_enum<FDirection>([&Data](FDirection i, bool& out_continue)
	{
		if (!Data.DebugEntityFlipbooks[i])
		{
			UE_LOG(LogTemp, Fatal, TEXT("Database doesn't contain debug flipbooks for entity"));
		}
	});
	if (!Data.DebugItemSprite)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Database doesn't contain debug sprite for item"));
	}
	if (!Data.DebugItemFlipbook)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Database doesn't contain debug flipbook for item"));
	}
	if (!Data.DebugProjectileSprite)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Database doesn't contain debug sprite for projectile"));
	}
	for_enum<FLiquidStatus>([&Data](FLiquidStatus i, bool& out_continue)
	{
		if (!Data.DebugLiquidFlipbooks.Contains(i))
		{
			Data.DebugLiquidFlipbooks.Add(i);
		}
	});
	for_enum<FLiquidStatus>([&Data](FLiquidStatus i, bool& out_continue)
	{
		if (!Data.DebugLiquidFlipbooks[i])
		{
			UE_LOG(LogTemp, Fatal, TEXT("Database doesn't contain debug flipbooks for liquid"));
		}
	});
	if (!Data.DebugSolidUnitSprite)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Database doesn't contain debug sprite for solid unit"));
	}
	if (!Data.DebugSolidUnitFlipbook)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Database doesn't contain debug flipbook for solid unit"));
	}
}
