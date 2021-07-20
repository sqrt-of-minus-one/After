    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Entity.h                    //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "../../Data/Database/EntityInfo.h"

#include "Entity.generated.h"

class UBoxComponent;
class UPaperFlipbookComponent;
class UPaperSpriteComponent;
class UAudioComponent;
class AItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDeathEvent, FDamageType, Type, AActor*, Murderer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FEntityDamageReceivedEvent, float, Value, FDamageType, Type, float, Direction, AActor*, FromWho, float, Push);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatsChangedEvent, float, NewValue);

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

	// Is called when entity's health changes
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FStatsChangedEvent OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetOxygen() const;

	// Is called when entity's oxygen changes
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FStatsChangedEvent OnOxygenChanged;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetEnergy() const;

	// Is called when entity's energy changes
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FStatsChangedEvent OnEnergyChanged;

			/* DAMAGE */

	// Deal damage to the entity
	UFUNCTION(Category = "Damage")
	virtual void Damage(float Value, FDamageType Type, float Direction, AActor* FromWho, float Push = 0.f);

	// Is called when entity receives a damage
	UPROPERTY(BlueprintAssignable, Category = "Damage")
	FEntityDamageReceivedEvent OnDamage;

	UFUNCTION(Category = "Damage")
	virtual void Stone(float Duration);

	UFUNCTION(Category = "Damage")
	virtual void Web(float Duration);

	UFUNCTION(Category = "Damage")
	bool IsDead();
	
	// Is called when entity dies
	UPROPERTY(BlueprintAssignable, Category = "Damage")
	FDeathEvent OnDeath;

protected:
			/* GENERAL */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FGameplayTag Id;

	const FEntityInfo* EntityData;

			/* EFFECTS */
	
	UPROPERTY(BlueprintReadOnly, Category = "Effects")
	bool bIsUnderPoison;

	UPROPERTY(BlueprintReadOnly, Category = "Effects")
	bool bIsUnderRadiation;

			/* MOVEMENT */

	// The entity's own movement
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector2D Moving;

	// Movement because of a push
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

	TDoubleLinkedList<const AEntity*> OverlappingEntities;

	UFUNCTION(Category = "Attack")
	void StartOverlap(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "Attack")
	void StopOverlap(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index);

			/* ATTACK */

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float LastAttackTime;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float LastAttackInterval;

	// If entity can miss, attack animation will be played even if target is 
	// too far. In this cast entity won't be able to attack for a while
	UFUNCTION(Category = "Attack")
	virtual bool MeleeAttack(AEntity* Target, bool bCanMiss, AItem* Weapon = nullptr);

	UFUNCTION(Category = "Attack")
	virtual void RangedAttack(FRotator Direction);

			/* DAMAGE */

	UFUNCTION(Category = "Damage")
	virtual void Death(FDamageType Type, AActor* Murderer);

	// Is called when death animation stops
	UFUNCTION(Category = "Damage")
	virtual void Disappear();

	UFUNCTION(Category = "Damage")
	virtual void DeathDrop();

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	bool bIsDead;

			/* STATS */

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Health;
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Oxygen;
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Energy;

	UFUNCTION(Category = "Stats")
	virtual void CalculateStats();

			/* SELECTION */

	// Entity is selected when it is under a mouse cursor
	UFUNCTION(Category = "Selection")
	void Select(AActor* Actor);

	// Entity is selected when it is under a mouse cursor
	UFUNCTION(Category = "Selection")
	void Unselect(AActor* Actor);

			/* APPEARANCE */
	
	UPROPERTY(BlueprintReadOnly, Category = "Appearance")
	bool bIsFlipbookFixed;
	
	UPROPERTY(BlueprintReadOnly, Category = "Appearance")
	FEntityStatus CurrentStatus;
	
	UPROPERTY(BlueprintReadOnly, Category = "Appearance")
	FDirection CurrentDirection;

	UFUNCTION(Category = "Appearance")
	virtual void SetFlipbook(FDirection Direction, FEntityStatus Status, float Time = 1.f);

	UFUNCTION(Category = "Appearance")
	void UnfixFlipbook();

			/* AUDIO */

	// Plays passive sound and sets timer for next playing
	UFUNCTION(Category = "Audio")
	void PlayPassiveSound();

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
};
