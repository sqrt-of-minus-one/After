    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: ExtraInfo.h                 //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "../Types.h"

#include "ExtraInfo.generated.h"

class UPaperSprite;
class USoundAttenuation;

USTRUCT(BlueprintType)
struct FExtraInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundAttenuation* SoundAttenuation;

	// Sprites that is used when player selects something
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FIntPoint, UPaperSprite*> SelectionSprites;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FDirection, UPaperFlipbook*> DebugEntityFlipbooks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPaperSprite* DebugItemSprite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPaperFlipbook* DebugItemFlipbook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPaperSprite* DebugProjectileSprite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FLiquidStatus, UPaperFlipbook*> DebugLiquidFlipbooks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPaperSprite* DebugSolidUnitSprite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPaperFlipbook* DebugSolidUnitFlipbook;
};
void Check(FExtraInfo& Data);
