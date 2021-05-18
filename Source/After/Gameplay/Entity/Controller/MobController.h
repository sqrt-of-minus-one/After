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

	UFUNCTION(Category = "Events")
	virtual void Damage(float Direction, AActor* FromWho); // Is called when the entity receives a damage

	UFUNCTION(Category = "Events")
	virtual void BeginDanger(const AActor* Actor); // Something has entered in danger sphere

	UFUNCTION(Category = "Events")
	virtual void EndDanger(const AActor* Actor); // Something has left danger sphere

	UFUNCTION(Category = "Events")
	virtual void BeginView(AActor* Actor); // Something has entered in view sphere

	UFUNCTION(Category = "Events")
	virtual void EndPursue(const AActor* Actor); // Something has left pursue sphere

			/* CONTROL */

	TDelegate<void(float)> MoveX;
	TDelegate<void(float)> MoveY;
	TDelegate<void()> StartRun;
	TDelegate<void()> StopRun;
	TDelegate<bool(AEntity*, bool)> Attack;

	UFUNCTION(Category = "Control")
	virtual void SetupInput();

protected:
			/* GENERAL */

	UPROPERTY(BlueprintReadOnly, Category = "General")
	AMob* MobPawn;

	UFUNCTION(Category = "General")
	void ClearTimers(AActor* Actor, EEndPlayReason::Type Reason);

			/* TARGETS */

	UPROPERTY(BlueprintReadOnly, Category = "Targets")
	AEntity* Target;
	
	TDoubleLinkedList<const AEntity*> RunningAwayFrom;
	TDoubleLinkedList<const AUnit*> DangerousUnits;

	UPROPERTY(BlueprintReadOnly, Category = "Targets")
	const AEntity* Attacker;

			/* TIMERS */

	UPROPERTY(BlueprintReadOnly, Category = "Timers")
	FTimerHandle ChangeStateTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Timers")
	FTimerHandle MobPainTimer;

			/* DELEGATES */

	FTimerDelegate ChangeStateDelegate;
	
			/* BEHAVIOUR */

	// Whether mob moves in specified direction
	UPROPERTY(BlueprintReadOnly, Category = "Behaviour")
	bool bHasTarget;

	UFUNCTION(Category = "Behaviour")
	void UpdateDirection();

	UFUNCTION(Category = "Behaviour")
	void StopPain();

	// When the direction was changed last time
	UPROPERTY(BlueprintReadOnly, Category = "Behaviour")
	float LastDirectionChangeTime;

			/* CONTROL */

	UFUNCTION(Category = "Control")
	void Move_f(FVector2D Val);

	UFUNCTION(Category = "Control")
	void SetRun_f(bool Val);

	UFUNCTION(Category = "Control")
	bool Attack_f(AEntity* TargetEntity);
};
