    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Robot.h                     //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Mob.h"

#include "Robot.generated.h"

UCLASS()
class AFTER_API ARobot : public AMob
{
	GENERATED_BODY()

public:
	ARobot();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

protected:
			/* GENERAL */

	const FRobotInfo* RobotData;

	UFUNCTION(BlueprintCallable, Category = "General")
	const FRobotInfo& GetRobotData() const;
};
