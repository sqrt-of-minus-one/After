    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Last.h                      //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Entity.h"

#include "Last.generated.h"

struct FLastInfo;
class ASolidUnit;
class USpringArmComponent;
class UCameraComponent;
//class UPlayerInventoryComponent;
//class USkillsComponent;

UCLASS()
class AFTER_API ALast : public AEntity
{
	GENERATED_BODY()

public:
	ALast();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

public:
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	const FLastInfo& GetLastData() const;
			
			/* STATS */

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetSatiety() const;

			/* TELEPATHY */

	UFUNCTION(Category = "Telepathy")
	void Weak();

protected:
			/* GENERAL */

	const FLastInfo* LastData;

			/* ZOOM */

	UFUNCTION(Category = "Zoom")
	void ZoomIn();

	UFUNCTION(Category = "Zoom")
	void ZoomOut();

			/* BREAKING */

	UPROPERTY(BlueprintReadOnly, Category = "Breaking")
	int DestroyerId;

	UPROPERTY(BlueprintReadOnly, Category = "Breaking")
	ASolidUnit* DestroyedUnit;

	UPROPERTY(BlueprintReadOnly, Category = "Breaking")
	AItem* ItemForBreaking;

	UFUNCTION(Category = "Breaking")
	void SetItemForBreaking(AItem* Item);

	UFUNCTION(Category = "Breaking")
	void StartBreak(ASolidUnit* Target, AItem* Item);

	UFUNCTION(Category = "Breaking")
	void StopBreak();

			/* DAMAGE */

	virtual void Death(FDamageType Type, const AActor* Murderer) override;
	
	virtual void Disappear() override;

			/* STATS */

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Satiety;

	virtual void CalculateStats() override;

			/* EVENTS */

//	UDELEGATE(Category = "Events")
	TDelegate<void()> DeathDelegate;

			/* COMPONENTS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
//	UPlayerInventoryComponent* InventoryComponent;

//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
//	USkillsComponent* SkillsComponent;

};
