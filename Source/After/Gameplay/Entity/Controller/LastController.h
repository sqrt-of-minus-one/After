    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: LastController.h            //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "LastController.generated.h"

class AEntity;
class ALast;
class ASolidUnit;
class AItem;
class UPlayerInventoryComponent;

DECLARE_DELEGATE_OneParam(FLastMovingDelegate, float);
DECLARE_DELEGATE(FLastRunningDelegate);
DECLARE_DELEGATE_RetVal_ThreeParams(bool, FLastAttackDelegate, AEntity*, bool, AItem*);
DECLARE_DELEGATE(FZoomDelegate);
DECLARE_DELEGATE_TwoParams(FStartBreakDelegate, ASolidUnit*, AItem*);
DECLARE_DELEGATE(FStopBreakDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChangedEvent, AItem*, NewItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectEvent, AActor*, Selected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnselectEvent, AActor*, Unselected);

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

	UPROPERTY(BlueprintAssignable, Category = "Selection")
	FOnSelectEvent OnSelect;

	UPROPERTY(BlueprintAssignable, Category = "Selection")
	FOnUnselectEvent OnUnselect;

			/* CONTROL */

	FLastMovingDelegate MoveX;
	FLastMovingDelegate MoveY;
	FZoomDelegate ZoomIn;
	FZoomDelegate ZoomOut;
	FLastRunningDelegate StartRun;
	FLastRunningDelegate StopRun;
	FLastAttackDelegate Attack;
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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Hotbar")
	int HotbarSlot;

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void HotbarItemChanged(int HotbarIndex, AItem* NewItem);

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

			/* DEBUG */

	UPROPERTY(BlueprintReadOnly, Category = "Debug")
	FTimerHandle DebugOutputTimer;

	UFUNCTION(Category = "Bebug")
	void DebugOutput();
};
