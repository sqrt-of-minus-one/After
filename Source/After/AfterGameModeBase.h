    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: AfterGameModeBase.h         //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "Data/Database.h"

#include "AfterGameModeBase.generated.h"

UCLASS()
class AFTER_API AAfterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	~AAfterGameModeBase();
	
protected:
	
	virtual void BeginPlay() override;

			/* DATA */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UDatabase* Database;

			/* CONSTANTS */

	const FVector TileSize = FVector(32.f, 32.f, 8.f);

	// How often stats are calculated
	const float CalcStatsInterval = 1.f;

public:
			/* DATA */

	const UDatabase* GetDatabase() const;

			/* CONSTANTS */

	FVector GetTileSize() const;

	// How often stats are calculated
	float GetCalcStatsInterval() const;
};
