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

public:
			/* DATA */

	const UDatabase* GetDatabase() const;

			/* CONSTANTS */

	static const FVector TileSize;
	static const float CalcStatsInterval;
	static const float PlayerSpringArmLength;

	FVector GetTileSize() const;
	float GetCalcStatsInterval() const;
	float GetPlayerSpringArmLength() const;
};
