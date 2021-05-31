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
	
	virtual void Damage(float Value, FDamageType Type, float Direction, AActor* FromWho, float Push = 0.f) override;

protected:
			/* GENERAL */

	const FMobInfo* MobData;

			/* VIEW */

	// Something has entered in the check danger box
	UFUNCTION(Category = "View")
	void BeginDanger(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
		bool bFromSweep, const FHitResult& SweepResult);

	// Something has left the check danger box
	UFUNCTION(Category = "View")
	void EndDanger(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index);

	// Something has entered in the view sphere
	UFUNCTION(Category = "View")
	void BeginView(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
		bool bFromSweep, const FHitResult& SweepResult);

	// Something has left the pursue sphere
	UFUNCTION(Category = "View")
	void EndPursue(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index);

			/* DAMAGE */

	virtual void DeathDrop() override;

			/* EVENTS */

//	UDELEGATE(Category = "Events")
	TDelegate<void(float, AActor*)> DamageDelegate;
	TDelegate<void(const AActor*)> BeginDangerDelegate;
	TDelegate<void(const AActor*)> EndDangerDelegate;
	TDelegate<void(AActor*)> BeginViewDelegate;
	TDelegate<void(AActor*)> EndPursueDelegate;

			/* COMPONENTS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* ViewComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* PursueComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CheckDangerBoxComponent;
};
