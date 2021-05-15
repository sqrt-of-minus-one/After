    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Unit.h                      //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "../../Data/Database/UnitInfo.h"

#include "Unit.generated.h"

class AEntity;
class AMob;
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
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FUnitInfo& GetUnitData() const;

	UFUNCTION(BlueprintCallable, Category = "General")
	const FGameplayTag& GetId() const;

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

			/* ATTACK */

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	FTimerHandle AttackTimer;

	// Entities that are being attacked by this unit
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	TArray<AEntity*> Attacked;

	// Add new entity to the Attacked array
	UFUNCTION(Category = "Attack")
	void StartAttack(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
		bool bFromSweep, const FHitResult& SweepResult);

	// Remove an entity from the Attacked array
	UFUNCTION(Category = "Attack")
	void StopAttack(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index);

	// Attack all of entities that are in the Attacked array
	UFUNCTION(Category = "Attack")
	void Attack();

			/* DANGER */

	UPROPERTY(BlueprintReadOnly, Category = "Danger")
	FTimerHandle DangerTimer;

	// Entities that are told about unit's danger
	UPROPERTY(BlueprintReadOnly, Category = "Danger")
	TArray<AMob*> DangerToEntities;

	// Add new entity to the DangerToEntities array
	UFUNCTION(Category = "Danger")
	void StartDanger(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
		bool bFromSweep, const FHitResult& SweepResult);

	// Remove an entity from the DangerToEntities array
	UFUNCTION(Category = "Danger")
	void StopDanger(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index);

	// Tell to all of entities that are in the DangerToEntities array, "I'm dangerous"
	UFUNCTION(Category = "Danger")
	void Danger();

			/* AUDIO */

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
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
	UBoxComponent* SeemsDangerousBoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComponent;
};
