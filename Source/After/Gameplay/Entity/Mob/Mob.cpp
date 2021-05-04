    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Mob.cpp                     //
////////////////////////////////////////

#include "Mob.h"

#include "../../LogGameplay.h"
#include "../../../Data/Database.h"
#include "../../../AfterGameModeBase.h"
#include "../Controller/MobController.h"

AMob::AMob()
{
	AIControllerClass = AMobController::StaticClass();
}

void AMob::BeginPlay()
{
	Super::BeginPlay();
	
	// Get game mode
	AAfterGameModeBase* GameMode = Cast<AAfterGameModeBase>(GetWorld()->GetAuthGameMode());
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
		MobController->StartRun.BindUObject(this, &AMob::StopRun);
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
