    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: AfterGameModeBase.h         //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "AfterGameModeBase.generated.h"

class UDatabase;
class ALangManager;

#define GAME_MODE Cast<AAfterGameModeBase>(GetWorld()->GetAuthGameMode())

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TSubclassOf<ALangManager> LangManagerClass;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	ALangManager* LangManager;

public:
			/* DATA */

	const UDatabase* GetDatabase() const;

	ALangManager* GetLangManager();
};
