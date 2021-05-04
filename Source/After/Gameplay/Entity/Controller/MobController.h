    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: MobController.h             //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"

#include "MobController.generated.h"

class AMob;

UCLASS()
class AFTER_API AMobController : public AAIController
{
	GENERATED_BODY()

public:
	AMobController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

public:
			/* CONTROL */

	TDelegate<void(float)> MoveX;
	TDelegate<void(float)> MoveY;
	TDelegate<void()> StartRun;
	TDelegate<void()> StopRun;

	virtual void SetupInput();

protected:
	AMob* MobPawn;

			/* TIMERS */

	FTimerHandle ChangeStateTimer;

			/* DELEGATES */

	FTimerDelegate ChangeStateDelegate;

			/* CONTROL */

	void Move_f(FVector2D Val);
	void SetRun_f(bool Val);
};
