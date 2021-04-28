    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Robot.cpp                   //
////////////////////////////////////////

#include "Robot.h"

#include "../../../Data/Database.h"
#include "../../../AfterGameModeBase.h"

ARobot::ARobot()
{

}

void ARobot::BeginPlay()
{
	Super::BeginPlay();
	
	// Get game mode
	AAfterGameModeBase* GameMode = Cast<AAfterGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Auth game mode is not AAfterGameModeBase"));
	}

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	RobotData = &Database->GetRobotData(Id);
}

void ARobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FRobotInfo& ARobot::GetRobotData() const
{
	return *RobotData;
}
