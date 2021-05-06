    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Liquid.cpp                  //
////////////////////////////////////////

#include "Liquid.h"

#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/AudioComponent.h"

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

	OnEndPlay.AddDynamic(this, &ALiquid::ClearTimers);

	// Get game mode
	AAfterGameModeBase* GameMode = Cast<AAfterGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogGameplay, Fatal, TEXT("Auth game mode is not AAfterGameModeBase"));
	}

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	LiquidData = &Database->GetLiquidData(Id);

	if (UnitData->bSelectable &&
		(!Database->GetExtraData().SelectionSprites.Contains(FIntPoint(1, 1)) ||
		!Database->GetExtraData().SelectionSprites[FIntPoint(1, 1)]))
	{
		UE_LOG(LogGameplay, Error, TEXT("Database doesn't contain selection sprite with size 1x1"));
	}
	else
	{
		SelectionSpriteComponent->SetSprite(Database->GetExtraData().SelectionSprites[FIntPoint(1, 1)]);
	}

	FlipbookComponent->SetFlipbook(LiquidData->Flipbooks[FLiquidStatus::Stay]);

	BeginFlow();
}

void ALiquid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		if (Amount == 0.f)
		{
			Destroy(); // There may be problems...
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

const FLiquidInfo& ALiquid::GetLiquidData() const
{
	return *LiquidData;
}

void ALiquid::Flow()
{
	// Todo
	PlaySound(FLiquidSoundType::Flow);
	GetWorld()->GetTimerManager().ClearTimer(FlowTimer);
}

void ALiquid::PlaySound(FLiquidSoundType Sound)
{
	int Size = LiquidData->Sounds.Sounds[Sound].Sounds.Num();
	if (Size != 0)
	{
		AudioComponent->SetSound(LiquidData->Sounds.Sounds[Sound].Sounds[FMath::RandRange(0, Size - 1)]);
		AudioComponent->Play();
	}
}
