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
	bHasGoal(false),
	LastDirectionChangeTime(0.f)
{
	PrimaryActorTick.bCanEverTick = false;
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

void AMobController::Damage(float Direction, AActor* FromWho)
{
	AEntity* EntityAttacker = Cast<AEntity>(FromWho);
	if (EntityAttacker)
	{
		Attacker = EntityAttacker;
		bHasGoal = true;
		SetRun_f(true);
		GetWorld()->GetTimerManager().SetTimer(MobPainTimer, this, &AMobController::StopPain, GameConstants::MobPainTime, false);
		
		const UDatabase* Database = GAME_MODE->GetDatabase();
		const FBehaviourProfileInfo& BehaviourProfileData = Database->GetBehaviourProfileData(MobPawn->GetMobData().BehaviourProfile);
		if (MobPawn->GetEntityData().Damage != 0 && !FBehaviourProfileInfo::bIsFearfulTowards(BehaviourProfileData, EntityAttacker->GetId(), Database))
		{
			Target = EntityAttacker;
		}

		ChangeStateDelegate.ExecuteIfBound();
	}
	else
	{
		const AUnit* UnitAttacker = Cast<AUnit>(FromWho);
		if (UnitAttacker && !DangerousUnits.Contains(UnitAttacker))
		{
			DangerousUnits.AddTail(UnitAttacker);
			bHasGoal = true;
			ChangeStateDelegate.ExecuteIfBound();
		}
	}
}

void AMobController::BeginDanger(const AActor* Actor)
{
	const AUnit* Unit = Cast<AUnit>(Actor);
	if (Unit && Unit->GetUnitData().bSeemsDangerous && !DangerousUnits.Contains(Unit))
	{
		DangerousUnits.AddTail(Unit);
		bHasGoal = true;
		ChangeStateDelegate.ExecuteIfBound();
	}
}

void AMobController::EndDanger(const AActor* Actor)
{
	const AUnit* Unit = Cast<AUnit>(Actor);
	if (Unit && DangerousUnits.Contains(Unit))
	{
		DangerousUnits.RemoveNode(Unit);
		if (!Target && RunningAwayFrom.Num() == 0 && !Attacker)
		{
			SetRun_f(false);
			if (DangerousUnits.Num() == 0)
			{
				bHasGoal = false;
			}
		}
	}
}

void AMobController::BeginView(AActor* Actor)
{
	AEntity* Entity = Cast<AEntity>(Actor);
	if (Entity && !Entity->IsDead())
	{
		const UDatabase* Database = GAME_MODE->GetDatabase();
		const FBehaviourProfileInfo& BehaviourProfileData = Database->GetBehaviourProfileData(MobPawn->GetMobData().BehaviourProfile);

		if (FBehaviourProfileInfo::bIsFearfulTowards(BehaviourProfileData, Entity->GetId(), Database) && !RunningAwayFrom.Contains(Entity))
		{
			RunningAwayFrom.AddTail(Entity);
			bHasGoal = true;
			SetRun_f(true);
			ChangeStateDelegate.ExecuteIfBound();
		}
		else if (FBehaviourProfileInfo::bIsAgressiveTowards(BehaviourProfileData, Entity->GetId(), Database) && MobPawn->GetEntityData().Damage != 0)
		{
			if (!Target)
			{
				Target = Entity;
				bHasGoal = true;
				SetRun_f(true);
				ChangeStateDelegate.ExecuteIfBound();
			}
			else if (!PossibleTargets.Contains(Entity))
			{
				PossibleTargets.AddTail(Entity);
			}
		}
	}
}

void AMobController::EndPursue(AActor* Actor)
{
	AEntity* Entity = Cast<AEntity>(Actor);
	if (Entity)
	{
		if (Target == Entity)
		{
			if (PossibleTargets.Num() == 0)
			{
				Target = nullptr;
			}
			else
			{
				Target = PossibleTargets.GetHead()->GetValue();
				PossibleTargets.RemoveNode(PossibleTargets.GetHead());
			}
		}
		if (Attacker == Entity)
		{
			Attacker = nullptr;
		}
		if (PossibleTargets.Contains(Entity))
		{
			PossibleTargets.RemoveNode(Entity);
		}
		if (RunningAwayFrom.Contains(Entity))
		{
			RunningAwayFrom.RemoveNode(Entity);
		}
		if (!Target && RunningAwayFrom.Num() == 0 && !Attacker)
		{
			SetRun_f(false);
			if (DangerousUnits.Num() == 0)
			{
				bHasGoal = false;
			}
		}
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
		UpdateDirection();

		GetWorld()->GetTimerManager().SetTimer(ChangeStateTimer, ChangeStateDelegate,
			bHasGoal ? GameConstants::MobUpdateDirectionTime : FMath::RandRange(GameConstants::MinMobChangeStateTime, GameConstants::MaxMobChangeStateTime), false);
	});
	GetWorld()->GetTimerManager().SetTimer(ChangeStateTimer, ChangeStateDelegate,
		FMath::RandRange(GameConstants::MinMobChangeStateTime, GameConstants::MaxMobChangeStateTime), false);
}

void AMobController::OnUnPossess()
{
	Super::OnUnPossess();

	ClearTimers(this, EEndPlayReason::Destroyed);
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

void AMobController::UpdateDirection()
{
	if (bHasGoal) // Mob should go to specific direction
	{
		FVector2D Direction(0.f, 0.f);
		if (RunningAwayFrom.Num() > 0 || DangerousUnits.Num() > 0 || (Attacker && !Target)) // Mob runs away from entities or dangerous units
		{
			for (const AEntity* i : RunningAwayFrom)
			{
				FVector2D Delta = FVector2D(MobPawn->GetActorLocation() - i->GetActorLocation());
				if (!Delta.IsZero())
				{
					Direction += Delta / (Delta.X * Delta.X + Delta.Y * Delta.Y);
				}
			}
			for (const AUnit* i : DangerousUnits)
			{
				FVector2D Delta = FVector2D(MobPawn->GetActorLocation() - i->GetActorLocation());
				if (!Delta.IsZero())
				{
					Direction += 2.f * Delta / (Delta.X * Delta.X + Delta.Y * Delta.Y);
				}
			}
			if (Attacker)
			{
				FVector2D Delta = FVector2D(MobPawn->GetActorLocation() - Attacker->GetActorLocation());
				if (!Delta.IsZero())
				{
					Direction += .75f * Delta / (Delta.X * Delta.X + Delta.Y * Delta.Y);
				}
			}
			Direction.Normalize();
			Move_f(Direction);
			return;
		}
		else if (Target)
		{
			Direction = FVector2D(Target->GetActorLocation() - MobPawn->GetActorLocation());
			Attack_f(Target);
			Direction.Normalize();
			Move_f(Direction);
			if (!Target || Target->IsDead())
			{
				if (PossibleTargets.Num() == 0)
				{
					Target = nullptr;
					if (RunningAwayFrom.Num() == 0 && !Attacker)
					{
						SetRun_f(false);
						if (DangerousUnits.Num() == 0)
						{
							bHasGoal = false;
						}
					}
				}
				else
				{
					Target = PossibleTargets.GetHead()->GetValue();
					PossibleTargets.RemoveNode(PossibleTargets.GetHead());
				}
			}
			return;
		}
	}

	// else

	if (FMath::RandBool())
	{
		Move_f(FVector2D(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1)));
	}
	else
	{
		Move_f(FVector2D(0.f, 0.f));
	}
}

void AMobController::StopPain()
{
	Attacker = nullptr;
	if (!Target && RunningAwayFrom.Num() == 0)
	{
		SetRun_f(false);
		if (DangerousUnits.Num() == 0)
		{
			bHasGoal = false;
		}
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
}

void AMobController::SetRun_f(bool Val)
{
	(Val ? StartRun : StopRun).ExecuteIfBound();
}

bool AMobController::Attack_f(AEntity* TargetEntity)
{
	if (Attack.IsBound())
	{
		return Attack.Execute(TargetEntity, false);
	}
	else
	{
		return false;
	}
}
