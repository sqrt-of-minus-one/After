    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: LastController.h            //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include <functional>

#include "GameFramework/PlayerController.h"

#include "LastController.generated.h"

class AEntity;
class ASolidUnit;
class AItem;

UCLASS()
class AFTER_API ALastController : public APlayerController
{
	GENERATED_BODY()

public:
	ALastController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

public:
			/* SELECTING */

	UFUNCTION()
	void Select(AActor* Actor);

	UFUNCTION()
	void Unselect(AActor* Actor);

			/* CONTROL */

	TDelegate<void(float)> MoveX;
	TDelegate<void(float)> MoveY;
	TDelegate<void()> ZoomIn;
	TDelegate<void()> ZoomOut;
	TDelegate<void()> StartRun;
	TDelegate<void()> StopRun;
	TDelegate<bool(AEntity*, bool, AItem*)> Attack;
	TDelegate<void(ASolidUnit*, AItem*)> StartBreak;
	TDelegate<void()> StopBreak;

	void SetupInput();

protected:
	AEntity* EntityPawn;

			/* SELECTING */

	AActor* Selected;

			/* STATE */

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsBreaking;

			/* INVENTORY */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	AItem* Item;

			/* CONTROL */

	void MoveX_f(float Value);
	void MoveY_f(float Value);
	void ZoomIn_f();
	void ZoomOut_f();
	void StartRun_f();
	void StopRun_f();
	void StartAttack_f();
	void StopAttack_f();
	void SpawnCow_tmp();
	void SwitchLang_tmp();
	void Throw_f();
};
