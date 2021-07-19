    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: LastController.h            //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "ControllerTypes.h"

#include "LastController.generated.h"

class ALast;
class ASolidUnit;
class UPlayerInventoryComponent;

DECLARE_DELEGATE(FZoomDelegate);
DECLARE_DELEGATE_TwoParams(FStartBreakDelegate, ASolidUnit*, AItem*);
DECLARE_DELEGATE(FStopBreakDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChangedEvent, AItem*, NewItem);

UENUM(BlueprintType)
enum class FMenuType : uint8
{
	Menu,
	Inventory,
	Crafting,
	Skills
};

UCLASS()
class AFTER_API ALastController : public APlayerController
{
	GENERATED_BODY()

public:
	ALastController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	// ==================================================

public:
			/* SELECTING */

	UFUNCTION(Category = "Selection")
	void Select(AActor* Actor);

	UFUNCTION(Category = "Selection")
	void Unselect(AActor* Actor);

			/* CONTROL */

	FMovingDelegate MoveX;
	FMovingDelegate MoveY;
	FZoomDelegate ZoomIn;
	FZoomDelegate ZoomOut;
	FRunningDelegate StartRun;
	FRunningDelegate StopRun;
	FAttackDelegate Attack;
	FStartBreakDelegate StartBreak;
	FStopBreakDelegate StopBreak;

			/* HOTBAR */

	UPROPERTY(BlueprintAssignable, Category = "Hotbar")
	FOnItemChangedEvent OnItemChanged;

	// Do not change item!
	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	AItem* GetSelectedItem();

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	int GetHotbarSlot();

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void SetHotbarSlot(int Slot);

protected:
			/* SELECTING */

	UPROPERTY(BlueprintReadOnly, Category = "Selecting")
	AActor* Selected;

			/* STATE */

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsBreaking;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsDead;

	UFUNCTION(Category = "State")
	void Death(FDamageType Type, AActor* Murderer); // Is called when the last dies

			/* HOTBAR */

	bool bWasItemValid;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Hotbar")
	int HotbarSlot;

			/* CONTROL */

	UPROPERTY(BlueprintReadOnly, Category = "Control")
	AEntity* EntityPawn;

	UPROPERTY(BlueprintReadOnly, Category = "Control")
	ALast* LastPawn;

	UPROPERTY(BlueprintReadOnly, Category = "Control")
	UPlayerInventoryComponent* Inventory;

	UFUNCTION(Category = "Control")
	void SetupInput();

	void MoveX_f(float Value);
	void MoveY_f(float Value);
	void ZoomIn_f();
	void ZoomOut_f();
	void StartRun_f();
	void StopRun_f();
	void StartAttack_f();
	void StopAttack_f();
	void Interact_f();
	void Throw_f();
	void Menu_f();
	void Inventory_f();
	void Crafting_f();
	void Skills_f();
	void SwitchLang_tmp();
};
