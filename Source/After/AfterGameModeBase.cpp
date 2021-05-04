    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: AfterGameModeBase.cpp       //
////////////////////////////////////////

#include "AfterGameModeBase.h"

#include "Data/LogDatabase.h"

AAfterGameModeBase::~AAfterGameModeBase()
{
	if (Database)
	{
		Database->Reset();
	}
}

void AAfterGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if (!Database)
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Couldn't find database"));
	}
	Database->Init();
}

const UDatabase* AAfterGameModeBase::GetDatabase() const
{
	return Database;
}