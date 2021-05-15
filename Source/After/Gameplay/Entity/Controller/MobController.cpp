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
	RunningAwayFrom(nullptr),
	bIsRunningAway(false),
	bPain(false),
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
		bPain = true;
		GetWorld()->GetTimerManager().SetTimer(MobPainTimer, this, &AMobController::StopPain, GameConstants::MobPainTime);
		
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
			(FMath::Abs(WhereToMove.X) > FMath::Abs(WhereToMove.Y) ? WhereToMove.Y : WhereToMove.X) = 0.f;
			if (bIsRunningAway && RunningAwayFrom)
			{
				FVector2D RunAwayDirection(MobPawn->GetActorLocation() - RunningAwayFrom->GetActorLocation());
				RunAwayDirection.Normalize();
				FVector2D OldWhereToMove(WhereToMove);
				WhereToMove += RunAwayDirection;
				if (WhereToMove.IsNearlyZero())
				{
					WhereToMove = OldWhereToMove.GetRotated(85.f);
				}
			}
			WhereToMove.Normalize();
			Move_f(WhereToMove);
		}
	}

	if (bShouldRunAway)
	{
		SetRun_f(true);
		Move_f(FVector2D(FMath::Cos(Direction), FMath::Sin(Direction)));
		bIsRunningAway = true;
		RunningAwayFrom = Attacker;
	}
	else
	{
		// Attack
	}
}

void AMobController::Danger(const AUnit* Detected)
{
	FVector2D WhereToMove(MobPawn->GetActorLocation() - Detected->GetActorLocation());
	if (bIsRunningAway && RunningAwayFrom)
	{
		FVector2D RunAwayDirection(MobPawn->GetActorLocation() - RunningAwayFrom->GetActorLocation());
		RunAwayDirection.Normalize();
		(FMath::Abs(WhereToMove.X) > FMath::Abs(WhereToMove.Y) ? WhereToMove.Y : WhereToMove.X) = 0.f;
		WhereToMove.Normalize();
		FVector2D OldWhereToMove(WhereToMove);
		WhereToMove += RunAwayDirection;
		if (WhereToMove.IsNearlyZero())
		{
			WhereToMove = OldWhereToMove.GetRotated(85.f);
		}
	}
	WhereToMove.Normalize();
	Move_f(WhereToMove);
}

void AMobController::BeginView(const AEntity* Entity)
{
	const UDatabase& Database = *(GAME_MODE->GetDatabase());
	const FBehaviourProfileInfo& BehaviourProfileData = Database.GetBehaviourProfileData(MobPawn->GetMobData().BehaviourProfile);

	if (FBehaviourProfileInfo::bIsFearfulTowards(BehaviourProfileData, Entity->GetId(), &Database))
	{
		FVector2D Direction(MobPawn->GetActorLocation() - Entity->GetActorLocation());
		Direction.Normalize();

		SetRun_f(true);
		Move_f(Direction);
		bIsRunningAway = true;
		RunningAwayFrom = Entity;
	}
}

void AMobController::EndPursue(const AEntity* Entity)
{
	if (RunningAwayFrom == Entity && !bPain)
	{
		StopPain();
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
		if (RunningAwayFrom && bIsRunningAway)
		{
			FVector2D Direction(MobPawn->GetActorLocation() - RunningAwayFrom->GetActorLocation());
			Direction.Normalize();
			Move_f(Direction);
		}
		else if (FMath::RandBool() || bIsRunningAway)
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
		if (GetWorld()->GetTimerManager().IsTimerActive(MobPainTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(MobPainTimer);
		}
	}
}

void AMobController::StopPain()
{
	bPain = false;
	if (bIsRunningAway)
	{
		RunningAwayFrom = nullptr;
		bIsRunningAway = false;
		SetRun_f(false);
	}
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
