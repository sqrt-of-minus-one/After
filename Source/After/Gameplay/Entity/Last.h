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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ==================================================

public:
			/* STATS */

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetSatiety() const;

			/* TELEPATHY */

	UFUNCTION()
	void Weak();

protected:
			/* GENERAL */

	const FLastInfo* LastData;

	UFUNCTION(BlueprintCallable, Category = "General")
	const FLastInfo& GetLastData() const;

			/* ZOOM */

	UFUNCTION()
	void ZoomIn();

	UFUNCTION()
	void ZoomOut();

			/* STATS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Satiety;

	virtual void CalculateStats() override;

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
