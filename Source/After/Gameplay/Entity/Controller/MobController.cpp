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
}

void AMobController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	MobPawn = Cast<AMob>(GetPawn());
	if (IsValid(MobPawn))
	{
		MobPawn->OnDamage.AddDynamic(this, &AMobController::Damage);
		MobPawn->OnBeginDanger.BindUObject(this, &AMobController::BeginDanger);
		MobPawn->OnEndDanger.BindUObject(this, &AMobController::EndDanger);
		MobPawn->OnBeginView.BindUObject(this, &AMobController::BeginView);
		MobPawn->OnEndPursue.BindUObject(this, &AMobController::EndPursue);

		GetWorld()->GetTimerManager().SetTimer(ChangeStateTimer, this, &AMobController::UpdateDirection,
			FMath::RandRange(GameConstants::MinMobChangeStateTime, GameConstants::MaxMobChangeStateTime), false);
	}
	else
	{
		UE_LOG(LogGameplay, Error, TEXT("Mob Controller: The pawn is not a mob or does not exist"));
		return;
	}
}

void AMobController::Damage(float Value, FDamageType Type, float Direction, AActor* FromWho, float Push)
{
	if (Value > 0.f)
	{
		AEntity* EntityAttacker = Cast<AEntity>(FromWho);
		if (IsValid(EntityAttacker)) // If attacker is entity
		{
			Attacker = EntityAttacker;
			bHasGoal = true;
			SetRun_f(true);
			FTimerHandle MobPainTimer;
			GetWorld()->GetTimerManager().SetTimer(MobPainTimer, this, &AMobController::StopPain, GameConstants::MobPainTime, false);
		
			const UDatabase* Database = GAME_MODE->GetDatabase();
			const FBehaviourProfileInfo& BehaviourProfileData = Database->GetBehaviourProfileData(MobPawn->GetMobData().BehaviourProfile);
			// If the mob can attack and doesn't fear attacker
			if (MobPawn->GetEntityData().Damage != 0 && !FBehaviourProfileInfo::bIsFearfulTowards(BehaviourProfileData, EntityAttacker->GetId(), Database))
			{
				Target = EntityAttacker;
			}

			UpdateDirection();
		}
		else
		{
			const AUnit* UnitAttacker = Cast<AUnit>(FromWho);
			// If attacker is unit
			if (IsValid(UnitAttacker) && !DangerousUnits.Contains(UnitAttacker))
			{
				DangerousUnits.AddTail(UnitAttacker);
				bHasGoal = true;

				UpdateDirection();
			}
		}
	}
}

void AMobController::BeginDanger(AActor* Actor)
{
	const AUnit* Unit = Cast<AUnit>(Actor);
	if (IsValid(Unit) && Unit->GetUnitData().bSeemsDangerous && !DangerousUnits.Contains(Unit))
	{
		DangerousUnits.AddTail(Unit);
		bHasGoal = true;
		UpdateDirection();
	}
}

void AMobController::EndDanger(AActor* Actor)
{
	const AUnit* Unit = Cast<AUnit>(Actor);
	if (IsValid(Unit) && DangerousUnits.Contains(Unit))
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
	if (IsValid(Entity) && !Entity->IsDead())
	{
		const UDatabase* Database = GAME_MODE->GetDatabase();
		const FBehaviourProfileInfo& BehaviourProfileData = Database->GetBehaviourProfileData(MobPawn->GetMobData().BehaviourProfile);

		if (FBehaviourProfileInfo::bIsFearfulTowards(BehaviourProfileData, Entity->GetId(), Database) && !RunningAwayFrom.Contains(Entity))
		{
			RunningAwayFrom.AddTail(Entity);
			bHasGoal = true;
			SetRun_f(true);
			UpdateDirection();
		}
		else if (MobPawn->GetEntityData().Damage != 0 && FBehaviourProfileInfo::bIsAgressiveTowards(BehaviourProfileData, Entity->GetId(), Database))
		{
			if (!IsValid(Target))
			{
				Target = Entity;
				bHasGoal = true;
				SetRun_f(true);
				UpdateDirection();
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
	if (IsValid(Entity))
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
		PossibleTargets.RemoveNode(Entity);
		RunningAwayFrom.RemoveNode(Entity);
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

void AMobController::UpdateDirection()
{
	if (bHasGoal) // Mob should go to specific direction
	{
		FVector2D Direction(0.f, 0.f);
		if (RunningAwayFrom.Num() > 0 || DangerousUnits.Num() > 0 || (Attacker && !Target)) // Mob is running away from entities or dangerous units
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
		}
		else if (IsValid(Target))
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
		}
	}
	else if (FMath::RandBool())
	{
		Move_f(FVector2D(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1)));
	}
	else
	{
		Move_f(FVector2D(0.f, 0.f));
	}
	
	GetWorld()->GetTimerManager().SetTimer(ChangeStateTimer, this, &AMobController::UpdateDirection,
		bHasGoal ? GameConstants::MobUpdateDirectionTime : FMath::RandRange(GameConstants::MinMobChangeStateTime, GameConstants::MaxMobChangeStateTime), false);
}

void AMobController::StopPain()
{
	Attacker = nullptr;
	if (!IsValid(Target) && RunningAwayFrom.Num() == 0)
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
		return Attack.Execute(TargetEntity, false, nullptr);
	}
	else
	{
		return false;
	}
}
