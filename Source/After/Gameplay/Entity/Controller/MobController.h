    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: MobController.h             //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"

#include "ControllerTypes.h"

#include "MobController.generated.h"

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

	virtual void OnPossess(APawn* InPawn) override;

	// ==================================================

public:
			/* CONTROL */

	FMovingDelegate MoveX;
	FMovingDelegate MoveY;
	FRunningDelegate StartRun;
	FRunningDelegate StopRun;
	FAttackDelegate Attack;

protected:
			/* GENERAL */

	UPROPERTY(BlueprintReadOnly, Category = "General")
	AMob* MobPawn;

			/* EVENTS */

	// Is called when the entity receives a damage
	UFUNCTION(Category = "Events")
	virtual void Damage(float Value, FDamageType Type, float Direction, AActor* FromWho, float Push);

	// Something has entered in danger sphere
	UFUNCTION(Category = "Events")
	virtual void BeginDanger(AActor* Actor);

	// Something has left danger sphere
	UFUNCTION(Category = "Events")
	virtual void EndDanger(AActor* Actor);

	// Something has entered in view sphere
	UFUNCTION(Category = "Events")
	virtual void BeginView(AActor* Actor);

	// Something has left pursue sphere
	UFUNCTION(Category = "Events")
	virtual void EndPursue(AActor* Actor);

			/* TARGETS */

	// The mob wants to attack its target
	UPROPERTY(BlueprintReadOnly, Category = "Targets")
	AEntity* Target;
	
	// Entities that can become targets when current target will be lost or will die
	TDoubleLinkedList<AEntity*> PossibleTargets;

	// The mob is running away from these entities
	TDoubleLinkedList<const AEntity*> RunningAwayFrom;

	// Dangerous units that are seen by the mob
	TDoubleLinkedList<const AUnit*> DangerousUnits;

	// The entity who has attacked the mob
	UPROPERTY(BlueprintReadOnly, Category = "Targets")
	AEntity* Attacker;

			/* BEHAVIOUR */

	// Whether mob is moving in specified direction
	UPROPERTY(BlueprintReadOnly, Category = "Behaviour")
	bool bHasGoal;

	UPROPERTY(BlueprintReadOnly, Category = "Behaviour")
	FTimerHandle ChangeStateTimer;

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
