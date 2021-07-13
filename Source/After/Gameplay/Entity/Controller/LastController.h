    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: LastController.h            //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include <functional>

#include "GameFramework/PlayerController.h"

#include "LastController.generated.h"

class AEntity;
class ASolidUnit;
class AItem;

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

	// ==================================================

public:
			/* SELECTING */

	UFUNCTION()
	void Select(AActor* Actor);

	UFUNCTION()
	void Unselect(AActor* Actor);

			/* EVENTS */

	UFUNCTION(Category = "Events")
	virtual void Death(); // Is called when the last becames dead

			/* CONTROL */

	TDelegate<void(float)> MoveX;
	TDelegate<void(float)> MoveY;
	TDelegate<void()> ZoomIn;
	TDelegate<void()> ZoomOut;
	TDelegate<void()> StartRun;
	TDelegate<void()> StopRun;
	TDelegate<bool(AEntity*, bool, AItem*)> Attack;
	TDelegate<void(ASolidUnit*, AItem*)> StartBreak;
	TDelegate<void()> StopBreak;
	TDelegate<void(AItem*)> SetItem;

	void SetupInput();

protected:
	AEntity* EntityPawn;

			/* SELECTING */

	UPROPERTY(BlueprintReadOnly, Category = "Selecting")
	AActor* Selected;

			/* STATE */

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsBreaking;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsDead;

			/* INVENTORY */

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	//AItem* Item;

			/* CONTROL */

	void MoveX_f(float Value);
	void MoveY_f(float Value);
	void ZoomIn_f();
	void ZoomOut_f();
	void StartRun_f();
	void StopRun_f();
	void StartAttack_f();
	void StopAttack_f();
	void Interact_f();
	void SwitchLang_tmp();
	void Throw_f();
	void Menu_f();
	void Inventory_f();
	void Crafting_f();
	void Skills_f();

	void OpenMenu(FMenuType Type);
};
