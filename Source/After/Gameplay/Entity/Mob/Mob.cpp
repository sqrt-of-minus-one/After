    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Mob.cpp                     //
////////////////////////////////////////

#include "Mob.h"

#include "../../LogGameplay.h"
#include "../../../Data/Database/Database.h"
#include "../../../AfterGameModeBase.h"
#include "../Controller/MobController.h"

AMob::AMob()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AMobController::StaticClass();
}

void AMob::BeginPlay()
{
	Super::BeginPlay();
	
	// Get game mode
	AAfterGameModeBase* GameMode = GAME_MODE;
	if (!GameMode)
	{
		UE_LOG(LogGameplay, Fatal, TEXT("Auth game mode is not AAfterGameModeBase"));
	}

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	MobData = &Database->GetMobData(Id);

	// Set controller
	AMobController* MobController = Cast<AMobController>(GetController());
	if (!MobController)
	{
		UE_LOG(LogGameplay, Error, TEXT("Mob (%s) doesn't have mob controller"), *Id.ToString());
	}
	else
	{
		MobController->MoveX.BindUObject(this, &AMob::SetMoveX);
		MobController->MoveY.BindUObject(this, &AMob::SetMoveY);
		MobController->StartRun.BindUObject(this, &AMob::StartRun);
		MobController->StopRun.BindUObject(this, &AMob::StopRun);

		DamageDelegate.BindUObject(MobController, &AMobController::Damage);
		DangerDelegate.BindUObject(MobController, &AMobController::Danger);

		MobController->SetupInput();
	}
}

void AMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FMobInfo& AMob::GetMobData() const
{
	return *MobData;
}

void AMob::Damage(float Value, FDamageType Type, float Direction, const AActor* FromWho, float Push)
{
	Super::Damage(Value, Type, Direction, FromWho, Push);

	DamageDelegate.ExecuteIfBound(Direction, FromWho);
}

void AMob::Danger(const AUnit* Unit)
{
	DangerDelegate.ExecuteIfBound(Unit);
}
