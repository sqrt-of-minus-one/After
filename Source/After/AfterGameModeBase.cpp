    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: AfterGameModeBase.cpp       //
////////////////////////////////////////

#include "AfterGameModeBase.h"

const FVector AAfterGameModeBase::TileSize = FVector(32.f, 32.f, 8.f);
const float AAfterGameModeBase::CalcStatsInterval = 1.f;

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
		UE_LOG(LogTemp, Fatal, TEXT("Couldn't find database"));
	}
	Database->Init();
}

const UDatabase* AAfterGameModeBase::GetDatabase() const
{
	return Database;
}

FVector AAfterGameModeBase::GetTileSize() const
{
	return TileSize;
}

float AAfterGameModeBase::GetCalcStatsInterval() const
{
	return CalcStatsInterval;
}
