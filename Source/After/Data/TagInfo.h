    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: TagInfo.h                   //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Types.h"

#include "TagInfo.generated.h"

USTRUCT(BlueprintType)
struct FTagInfo
{
	GENERATED_USTRUCT_BODY()

public:
			/* DATA */

	// What is contained in tag
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TArray<FGameplayTag> Objects;
};