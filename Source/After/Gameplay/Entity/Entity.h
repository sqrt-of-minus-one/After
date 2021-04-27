    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Entity.h                    //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "../../Data/EntityInfo.h"

#include "Entity.generated.h"

struct FEntityInfo;
class UBoxComponent;
class UPaperFlipbookComponent;
class UPaperSpriteComponent;
class UAudioComponent;

UCLASS()
class AFTER_API AEntity : public APawn
{
	GENERATED_BODY()

public:
	AEntity();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

// ==================================================

public:
			/* STATS */

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetOxygen() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetEnergy() const;

			/* DAMAGE */

	UFUNCTION()
	virtual void Damage(float Value, FDamageType Type, const AActor* FromWho);

	UFUNCTION()
	virtual void Stone(float Duration);

	UFUNCTION()
	virtual void Web(float Duration);

			/* SELECTION */

	UFUNCTION()
	void Select();

	UFUNCTION()
	void Unselect();

protected:
			/* GENERAL */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FGameplayTag Id;

	const FEntityInfo* EntityData;

	UFUNCTION(BlueprintCallable, Category = "General")
	const FEntityInfo& GetEntityData() const;

			/* EFFECTS */
	
	// Time remaining until the end of poison effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FTimerHandle PoisonTimer;

	// Time remaining until the end of radiation effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FTimerHandle RadiationTimer;

			/* MOVEMENT */

	UFUNCTION()
	void SetMoveX(float Value);

	UFUNCTION()
	void SetMoveY(float Value);

	UFUNCTION()
	virtual void StartRun();

	UFUNCTION()
	virtual void StopRun();

			/* ATTACK */

	UFUNCTION()
	virtual bool MeleeAttack(AEntity* Target);

	UFUNCTION()
	virtual void RangedAttack(FRotator Direction);

			/* DAMAGE */

	UFUNCTION()
	virtual void Death(FDamageType Type, const AActor* Murderer);

	UFUNCTION()
	virtual void DeathDrop();

			/* DEATH */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Death")
	bool bIsDead;

			/* STATS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	FTimerHandle StatsTimer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Oxygen;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Energy;

	UFUNCTION()
	virtual void CalculateStats();

			/* MOVEMENT */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	FVector2D Moving;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsRunning;

	UFUNCTION()
	void Move(float DeltaTime);

			/* APPEARANCE */
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FTimerHandle FixedFlipbookTimer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FEntityStatus CurrentStatus;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FDirection CurrentDirection;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	bool bIsFlipbookFixed;

	UFUNCTION()
	virtual void SetFlipbook(FDirection Direction, FEntityStatus Status, float Time = 1.f);

			/* AUDIO */
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	FTimerHandle AudioTimer;

	UFUNCTION()
	virtual void PlaySound(FEntitySoundType Sound);

			/* COMPONENTS */
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperFlipbookComponent* FlipbookComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* SelectionSpriteComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComponent;

private:
	FTimerDelegate AudioDelegate;
};
