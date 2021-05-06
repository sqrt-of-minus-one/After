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
			/* EVENTS */

	void Damage(float Direction, const AActor* FromWho);

			/* CONTROL */

	TDelegate<void(float)> MoveX;
	TDelegate<void(float)> MoveY;
	TDelegate<void()> StartRun;
	TDelegate<void()> StopRun;

	virtual void SetupInput();

protected:
	AMob* MobPawn;

	UFUNCTION()
	void ClearTimers(AActor* Actor, EEndPlayReason::Type Reason);

			/* TIMERS */

	FTimerHandle ChangeStateTimer;
	FTimerHandle RunAwayTimer;
	
			/* BEHAVIOUR */

	UFUNCTION(Category = "Behaviour")
	void StopRunAway();

	UPROPERTY(BlueprintReadOnly, Category = "Behaviour")
	bool bIsRunningAway;

			/* DELEGATES */

	FTimerDelegate ChangeStateDelegate;

			/* CONTROL */

	void Move_f(FVector2D Val);
	void SetRun_f(bool Val);
};
