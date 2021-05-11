    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: MobController.cpp           //
////////////////////////////////////////

#include "MobController.h"

#include "Kismet/KismetSystemLibrary.h"

#include "../../../Data/Database/Database.h"
#include "../../LogGameplay.h"
#include "../../../AfterGameModeBase.h"
#include "../../../GameConstants.h"
#include "../Mob/Mob.h"
#include "../../Unit/Unit.h"

AMobController::AMobController() :
	bIsRunningAway(false),
	LastDirectionChangeTime(0.f)
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
	const AEntity* Attacker = Cast<AEntity>(FromWho);
	bool bShouldRunAway = true;

	if (Attacker)
	{
		const UDatabase& Database = *(GAME_MODE->GetDatabase());
		const FBehaviourProfileInfo& BehaviourProfileData = Database.GetBehaviourProfileData(MobPawn->GetMobData().BehaviourProfile);

		bool bFear = FBehaviourProfileInfo::bIsFearfulTowards(BehaviourProfileData, Attacker->GetId(), &Database);

		// Mob should run away if it cannot attack or if it fears attacker
		bShouldRunAway = (MobPawn->GetEntityData().Damage == 0 || bFear);
	}
	else
	{
		const AUnit* UnitAttacker = Cast<AUnit>(FromWho);
		if (UnitAttacker)
		{
			bShouldRunAway = false;
			FVector2D WhereToMove(MobPawn->GetActorLocation() - UnitAttacker->GetActorLocation());
			WhereToMove.Normalize();
			Move_f(WhereToMove);
		}
	}

	if (bShouldRunAway)
	{

		SetRun_f(true);
		Move_f(FVector2D(FMath::Cos(Direction), FMath::Sin(Direction)));
		bIsRunningAway = true;
		GetWorld()->GetTimerManager().SetTimer(RunAwayTimer, this, &AMobController::StopRunAway, 5.f);
	}
	else
	{
		// Attack
	}
}

void AMobController::Danger(const AUnit* Detected)
{
	if (!bIsRunningAway)
	{
		FVector2D WhereToMove(MobPawn->GetActorLocation() - Detected->GetActorLocation());
		WhereToMove.Normalize();
		Move_f(WhereToMove);
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
	float CurrentGameTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
	if (Val.IsZero() || CurrentGameTime - LastDirectionChangeTime > GameConstants::MinMobChangeDirectionTime)
	{
		MoveX.ExecuteIfBound(Val.X);
		MoveY.ExecuteIfBound(Val.Y);
		LastDirectionChangeTime = CurrentGameTime;
	}
	else if (CurrentGameTime - LastDirectionChangeTime > 0.f)
	{
		if (!bIsRunningAway)
		{
			MoveX.ExecuteIfBound(0.f);
			MoveY.ExecuteIfBound(0.f);
		}
		ChangeDirectionDelegate.BindLambda([this, Val]()
		{
			Move_f(Val);
		});
		GetWorld()->GetTimerManager().SetTimer(ChangeDirectionTimer, ChangeDirectionDelegate, CurrentGameTime - LastDirectionChangeTime, false);
	}
}

void AMobController::SetRun_f(bool Val)
{
	(Val ? StartRun : StopRun).ExecuteIfBound();
}
