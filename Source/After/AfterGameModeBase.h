    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: AfterGameModeBase.h         //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "AfterGameModeBase.generated.h"

class UUserWidget;
class ACrate;
class ALast;
class UDatabase;
class ALangManager;
class AWidgetInitializer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEntityStatsEvent);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TSubclassOf<AWidgetInitializer> WidgetInitializerClass;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	AWidgetInitializer* WidgetInitializer;

public:
			/* DATA */

	const UDatabase* GetDatabase() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	ALangManager* GetLangManager();

	UFUNCTION(BlueprintCallable, Category = "Data")
	AWidgetInitializer* GetWidgetInitializer();

			/* ENTITIES */

	// Is called when entities should calculate their stats
	UPROPERTY(BlueprintAssignable, Category = "Entities")
	FEntityStatsEvent OnEntityStats;

	UPROPERTY(BlueprintReadOnly, Category = "Entities")
	FTimerHandle EntityStatsTimer;

	UFUNCTION(Category = "Entities")
	void EntityStatsExec();

};
