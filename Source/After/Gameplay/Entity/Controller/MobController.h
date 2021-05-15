    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: MobController.h             //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"

#include "MobController.generated.h"

class AEntity;
class AMob;
class AUnit;

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

	virtual void Damage(float Direction, const AActor* FromWho); // Is called when the entity receives a damage
	virtual void Danger(const AUnit* Detected); // Is called when the entity is near a unit that seems dangerous
	virtual void BeginView(const AEntity* Entity); // Entity has entered in view sphere
	virtual void EndPursue(const AEntity* Entity); // Entity has left pursue sphere

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

			/* TARGETS */

	UPROPERTY(BlueprintReadOnly, Category = "Targets")
	const AEntity* RunningAwayFrom;

			/* TIMERS */

	FTimerHandle ChangeDirectionTimer;
	FTimerHandle ChangeStateTimer;
	FTimerHandle MobPainTimer;
	
			/* BEHAVIOUR */

	UFUNCTION(Category = "Behaviour")
	void StopPain();

	UPROPERTY(BlueprintReadOnly, Category = "Behaviour")
	bool bIsRunningAway;

	UPROPERTY(BlueprintReadOnly, Category = "Behaviour")
	bool bPain;

	// When the direction was changed last time
	UPROPERTY(BlueprintReadOnly, Category = "Behaviour")
	float LastDirectionChangeTime;

			/* DELEGATES */

	FTimerDelegate ChangeDirectionDelegate;
	FTimerDelegate ChangeStateDelegate;

			/* CONTROL */

	void Move_f(FVector2D Val);
	void SetRun_f(bool Val);
};
