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
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FEntityInfo& GetEntityData() const;

	UFUNCTION(BlueprintCallable, Category = "General")
	const FGameplayTag& GetId() const;

			/* STATS */

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetOxygen() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetEnergy() const;

			/* DAMAGE */

	UFUNCTION(Category = "Damage")
	virtual void Damage(float Value, FDamageType Type, float Direction, const AActor* FromWho, float Push = 0.f);

	UFUNCTION(Category = "Damage")
	virtual void Stone(float Duration);

	UFUNCTION(Category = "Damage")
	virtual void Web(float Duration);

			/* SELECTION */

	UFUNCTION(Category = "Selection")
	void Select();

	UFUNCTION(Category = "Selection")
	void Unselect();

protected:
	UFUNCTION()
	void ClearTimers(AActor* Actor, EEndPlayReason::Type Reason);

			/* GENERAL */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FGameplayTag Id;

	const FEntityInfo* EntityData;

			/* EFFECTS */
	
	// Time remaining until the end of poison effect
	UPROPERTY(BlueprintReadOnly, Category = "Effects")
	FTimerHandle PoisonTimer;

	// Time remaining until the end of radiation effect
	UPROPERTY(BlueprintReadOnly, Category = "Effects")
	FTimerHandle RadiationTimer;

			/* MOVEMENT */

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector2D Moving;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector2D PushMoving;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsRunning;

	UFUNCTION(Category = "Movement")
	void Move(float DeltaTime);

	UFUNCTION(Category = "Movement")
	void AddOffset(FVector Offset);

	UFUNCTION(Category = "Movement")
	void SetMoveX(float Value);

	UFUNCTION(Category = "Movement")
	void SetMoveY(float Value);

	UFUNCTION(Category = "Movement")
	virtual void StartRun();

	UFUNCTION(Category = "Movement")
	virtual void StopRun();

			/* ATTACK */

	UFUNCTION(Category = "Attack")
	virtual bool MeleeAttack(AEntity* Target);

	UFUNCTION(Category = "Attack")
	virtual void RangedAttack(FRotator Direction);

			/* DAMAGE */

	UFUNCTION(Category = "Damage")
	virtual void Death(FDamageType Type, const AActor* Murderer);

	UFUNCTION(Category = "Damage")
	virtual void DeathDrop();

			/* DEATH */

	UPROPERTY(BlueprintReadOnly, Category = "Death")
	bool bIsDead;

			/* STATS */

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FTimerHandle StatsTimer;
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Health;
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Oxygen;
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Energy;

	UFUNCTION(Category = "Stats")
	virtual void CalculateStats();

			/* APPEARANCE */
	
	UPROPERTY(BlueprintReadOnly, Category = "Appearance")
	FTimerHandle FixedFlipbookTimer;
	
	UPROPERTY(BlueprintReadOnly, Category = "Appearance")
	FEntityStatus CurrentStatus;
	
	UPROPERTY(BlueprintReadOnly, Category = "Appearance")
	FDirection CurrentDirection;
	
	UPROPERTY(BlueprintReadOnly, Category = "Appearance")
	bool bIsFlipbookFixed;

	UFUNCTION(Category = "Appearance")
	virtual void SetFlipbook(FDirection Direction, FEntityStatus Status, float Time = 1.f);

			/* AUDIO */
	
	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	FTimerHandle AudioTimer;

	UFUNCTION(Category = "Audio")
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
