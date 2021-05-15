    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Mob.h                       //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "../Entity.h"

#include "Mob.generated.h"

class AUnit;
class USphereComponent;

UCLASS()
class AFTER_API AMob : public AEntity
{
	GENERATED_BODY()

public:
	AMob();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

public:
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FMobInfo& GetMobData() const;

			/* DAMAGE */
	
	virtual void Damage(float Value, FDamageType Type, float Direction, const AActor* FromWho, float Push = 0.f) override;

	UFUNCTION(Category = "Damage")
	virtual void Danger(const AUnit* Unit);

protected:
			/* GENERAL */

	const FMobInfo* MobData;

			/* VIEW */

	// Something has entered in the view sphere
	UFUNCTION(Category = "View")
	void BeginView(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
		bool bFromSweep, const FHitResult& SweepResult);

	// Something has left the pursue sphere
	UFUNCTION(Category = "View")
	void EndPursue(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index);

			/* EVENTS */

//	UDELEGATE(Category = "Events")
	TDelegate<void(float, const AActor*)> DamageDelegate;
	TDelegate<void(const AUnit*)> DangerDelegate;
	TDelegate<void(const AEntity*)> BeginViewDelegate;
	TDelegate<void(const AEntity*)> EndPursueDelegate;

			/* COMPONENTS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* ViewComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* PursueComponent;
};
