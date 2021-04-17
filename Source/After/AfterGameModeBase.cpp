    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: AfterGameModeBase.cpp       //
////////////////////////////////////////

#include "AfterGameModeBase.h"

void AAfterGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Database->Check();
}

const UDatabase* AAfterGameModeBase::GetDatabase() const
{
	return Database;
}