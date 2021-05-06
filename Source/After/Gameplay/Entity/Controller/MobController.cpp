    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: MobController.cpp           //
////////////////////////////////////////

#include "MobController.h"

#include "../../LogGameplay.h"
#include "../../../GameConstants.h"
#include "../Mob/Mob.h"

AMobController::AMobController()
{

}

void AMobController::BeginPlay()
{
	Super::BeginPlay();

	OnEndPlay.AddDynamic(this, &AMobController::ClearTimers);
}

void AMobController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobController::Damage(float Direction, const AActor* FromWho)
{
	if (MobPawn->GetEntityData().Damage != 0)
	{

	}
	else
	{
		SetRun_f(true);
		Move_f(FVector2D(FMath::Cos(Direction), FMath::Sin(Direction)));
		bIsRunningAway = true;
		GetWorld()->GetTimerManager().SetTimer(RunAwayTimer, this, &AMobController::StopRunAway, 5.f);
	}
}

void AMobController::SetupInput()
{
	MobPawn = Cast<AMob>(GetPawn());
	if (!MobPawn)
	{
		UE_LOG(LogGameplay, Error, TEXT("Mob Controller: The pawn is not a mob"));
		return;
	}

	ChangeStateDelegate.BindLambda([this]()
	{
		if (FMath::RandBool() || bIsRunningAway)
		{
			Move_f(FVector2D(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1)));
		}
		else
		{
			Move_f(FVector2D(0.f, 0.f));
		}

		GetWorld()->GetTimerManager().SetTimer(ChangeStateTimer, ChangeStateDelegate, FMath::RandRange(GameConstants::MinMobChangeStateTime, GameConstants::MaxMobChangeStateTime), false);
	});
	GetWorld()->GetTimerManager().SetTimer(ChangeStateTimer, ChangeStateDelegate, FMath::RandRange(GameConstants::MinMobChangeStateTime, GameConstants::MaxMobChangeStateTime), false);

}

void AMobController::ClearTimers(AActor* Actor, EEndPlayReason::Type Reason)
{
	if (GetWorld())
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(ChangeStateTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(ChangeStateTimer);
		}
		if (GetWorld()->GetTimerManager().IsTimerActive(RunAwayTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(RunAwayTimer);
		}
	}
}

void AMobController::StopRunAway()
{
	bIsRunningAway = false;
	SetRun_f(false);
}

void AMobController::Move_f(FVector2D Val)
{
	MoveX.ExecuteIfBound(Val.X);
	MoveY.ExecuteIfBound(Val.Y);
}

void AMobController::SetRun_f(bool Val)
{
	(Val ? StartRun : StopRun).ExecuteIfBound();
}
