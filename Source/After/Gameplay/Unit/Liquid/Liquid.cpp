    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Liquid.cpp                  //
////////////////////////////////////////

#include "Liquid.h"

#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"

#include "../../../Data/Database/Database.h"
#include "../../LogGameplay.h"
#include "../../../AfterGameModeBase.h"
#include "../../../GameConstants.h"

ALiquid::ALiquid()
{
	CollisionComponent->SetCollisionProfileName(FName("Liquid"));
}

void ALiquid::BeginPlay()
{
	Super::BeginPlay();

	// Get database
	const UDatabase* Database = GAME_MODE->GetDatabase();
	LiquidData = &Database->GetLiquidData(Id);

	if (UnitData->bSelectable)
	{
		if (!Database->GetExtraData().SelectionSprites.Contains(FIntPoint(1, 1)) ||
			!Database->GetExtraData().SelectionSprites[FIntPoint(1, 1)])
		{
			UE_LOG(LogGameplay, Error, TEXT("Database doesn't contain selection sprite with size 1x1"));
		}
		else
		{
			SelectionSpriteComponent->SetSprite(Database->GetExtraData().SelectionSprites[FIntPoint(1, 1)]);
		}
	}

	FlipbookComponent->SetFlipbook(LiquidData->Flipbooks[FLiquidStatus::Stay]);

	BeginFlow();
}

void ALiquid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FLiquidInfo& ALiquid::GetLiquidData() const
{
	return *LiquidData;
}

float ALiquid::Add(FGameplayTag Liquid, float AddedAmount)
{
	if (Liquid == Id && AddedAmount > 0)
	{
		float OldAmount = Amount;
		Amount = FMath::Clamp(Amount + AddedAmount, 0.f, GameConstants::LiquidValueInTile);
		BeginFlow();
		return Amount - OldAmount;
	}
	else
	{
		return 0.f;
	}
}

float ALiquid::Get(FGameplayTag Liquid, float GotAmount)
{
	if (Liquid == Id && GotAmount > 0)
	{
		float Got = FMath::Min(GotAmount, Amount);
		Amount -= Got;
		if (Amount <= 0.f)
		{
			GetWorld()->DestroyActor(this);
		}
		return Got;
	}
	else
	{
		return 0.f;
	}
}

void ALiquid::BeginFlow()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(FlowTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(FlowTimer, this, &ALiquid::Flow, 1 / LiquidData->Speed, true);
	}
}

void ALiquid::ClearTimers(AActor* Actor, EEndPlayReason::Type Reason)
{
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(FlowTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(FlowTimer);
	}
}

void ALiquid::Flow()
{
	// Todo
	GetWorld()->GetTimerManager().ClearTimer(FlowTimer);
}
