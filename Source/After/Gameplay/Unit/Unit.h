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

protected:
			/* GENERAL */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FGameplayTag Id;

	const FUnitInfo* UnitData;

			/* SELECTION */

	UFUNCTION(Category = "Selection")
	void Select(AActor* Actor);

	UFUNCTION(Category = "Selection")
	void Unselect(AActor* Actor);

			/* ATTACK */

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	FTimerHandle AttackTimer;

	TDoubleLinkedList<AEntity*> Attacked;

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

			/* COMPONENTS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* SelectionSpriteComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* DamageBoxComponent;
};
