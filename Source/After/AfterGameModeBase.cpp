    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: AfterGameModeBase.cpp       //
////////////////////////////////////////

#include "AfterGameModeBase.h"

const FVector AAfterGameModeBase::TileSize = FVector(16.f, 16.f, 4.f);
const FVector AAfterGameModeBase::DamageBoxDelta = FVector(1.f, 1.f, 0.f);
const float AAfterGameModeBase::CalcStatsInterval = 1.f;
const float AAfterGameModeBase::UnitDamageInterval = 1.f;
const float AAfterGameModeBase::ZoomStep = 1.1f;
const float AAfterGameModeBase::PlayerSpringArmLength = 250.f;
const float AAfterGameModeBase::MinPlayerSpringArmLength = 100.f;
const float AAfterGameModeBase::MaxPlayerSpringArmLength = 1500.f;

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