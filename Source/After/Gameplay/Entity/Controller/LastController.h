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
	TDelegate<bool(AEntity*)> Attack;

	void SetupInput();

protected:
			/* SELECTING */

	AActor* Selected;

			/* CONTROL */

	void MoveX_f(float Value);
	void MoveY_f(float Value);
	void ZoomIn_f();
	void ZoomOut_f();
	void StartRun_f();
	void StopRun_f();
	void Attack_f();
	void SpawnCow_tmp();
};
