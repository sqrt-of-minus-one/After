    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Unit.h                      //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "../../Data/UnitInfo.h"

#include "Unit.generated.h"

class AEntity;
class UBoxComponent;
class UPaperFlipbookComponent;
class UPaperSpriteComponent;
class UAudioComponent;

UCLASS()
class AFTER_API AUnit : public AActor
{
	GENERATED_BODY()

public:
	AUnit();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

public:
			/* SELECTION */

	UFUNCTION()
	void Select();

	UFUNCTION()
	void Unselect();

protected:
	UFUNCTION()
	void ClearTimers(AActor* Actor, EEndPlayReason::Type Reason);

			/* GENERAL */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FGameplayTag Id;

	const FUnitInfo* UnitData;

	UFUNCTION(BlueprintCallable, Category = "General")
	const FUnitInfo& GetUnitData() const;

			/* ATTACK */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	FTimerHandle AttackTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TArray<AEntity*> Attacked;

	UFUNCTION()
	void StartAttack(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void StopAttack(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index);

	UFUNCTION()
	void Attack();

			/* AUDIO */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	FTimerHandle AudioTimer;

			/* COMPONENTS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* SelectionSpriteComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* DamageBoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComponent;
};
