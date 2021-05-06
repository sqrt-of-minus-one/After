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
		if (FMath::RandBool())
		{
			Move_f(FVector2D(0.f, 0.f));
		}
		else
		{
			Move_f(FVector2D(FMath::RandRange(-1, 1),
							 FMath::RandRange(-1, 1)));
		}

		GetWorld()->GetTimerManager().SetTimer(ChangeStateTimer, ChangeStateDelegate, FMath::RandRange(GameConstants::MinMobChangeStateTime, GameConstants::MaxMobChangeStateTime), false);
	});
	GetWorld()->GetTimerManager().SetTimer(ChangeStateTimer, ChangeStateDelegate, FMath::RandRange(GameConstants::MinMobChangeStateTime, GameConstants::MaxMobChangeStateTime), false);

}

void AMobController::ClearTimers(AActor* Actor, EEndPlayReason::Type Reason)
{
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(ChangeStateTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(ChangeStateTimer);
	}
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
