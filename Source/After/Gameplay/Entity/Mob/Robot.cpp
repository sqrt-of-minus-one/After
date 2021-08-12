    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Robot.cpp                   //
////////////////////////////////////////

#include "Robot.h"

#include "../../LogGameplay.h"
#include "../../../Data/Database/Database.h"
#include "../../../AfterGameModeBase.h"

ARobot::ARobot()
{

}

void ARobot::BeginPlay()
{
	Super::BeginPlay();

	// Get database
	const UDatabase* Database = GAME_MODE->GetDatabase();
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
