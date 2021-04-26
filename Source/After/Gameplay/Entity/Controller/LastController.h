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

DECLARE_DELEGATE_OneParam(FAxisDelegate, float);
DECLARE_DELEGATE(FEventDelegate);

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

			/* CONTROLLING */

	FAxisDelegate MoveX;
	FAxisDelegate MoveY;
	FEventDelegate ZoomIn;
	FEventDelegate ZoomOut;
	FEventDelegate StartRun;
	FEventDelegate StopRun;

	void SetupInput();

protected:
			/* SELECTING */

	AActor* Selected;

			/* CONTROLLING */

	void MoveX_f(float Value);
	void MoveY_f(float Value);
	void ZoomIn_f();
	void ZoomOut_f();
	void StartRun_f();
	void StopRun_f();
};
