    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Unit.cpp                    //
////////////////////////////////////////

#include "Unit.h"

#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/AudioComponent.h"

#include "../LogGameplay.h"
#include "../Entity/Controller/LastController.h"
#include "../../AfterGameModeBase.h"
#include "../Entity/Entity.h"
#include "../../GameConstants.h"

AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetBoxExtent(GameConstants::TileSize);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	FlipbookComponent->SetupAttachment(GetRootComponent());

	SelectionSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Selection Sprite"));
	SelectionSpriteComponent->SetupAttachment(FlipbookComponent);
	SelectionSpriteComponent->SetVisibility(false);

	DamageBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Box"));
	DamageBoxComponent->SetupAttachment(GetRootComponent());
	DamageBoxComponent->SetBoxExtent(GameConstants::TileSize + GameConstants::DamageBoxDelta);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->SetupAttachment(GetRootComponent());
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();

	OnEndPlay.AddDynamic(this, &AUnit::ClearTimers);

	// Get game mode
	AAfterGameModeBase* GameMode = Cast<AAfterGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogGameplay, Fatal, TEXT("Auth game mode is not AAfterGameModeBase"));
	}

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	UnitData = &Database->GetUnitData(Id);
	
	FlipbookComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	FlipbookComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	SelectionSpriteComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	DamageBoxComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	AudioComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	ALastController* LastController = Cast<ALastController>(GetWorld()->GetFirstPlayerController());
	if (LastController)
	{
		if (UnitData->bSelectable)
		{
			OnBeginCursorOver.AddDynamic(LastController, &ALastController::Select);
			OnEndCursorOver.AddDynamic(LastController, &ALastController::Unselect);
		}
		else
		{
			SelectionSpriteComponent->DestroyComponent();
			SelectionSpriteComponent = nullptr;
		}
	}
	else
	{
		UE_LOG(LogGameplay, Error, TEXT("Couldn't find Last Controller"));
	}

	if (UnitData->Damage != 0.f)
	{
		DamageBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUnit::StartAttack);
		DamageBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AUnit::StopAttack);
	}
	else
	{
		DamageBoxComponent->DestroyComponent();
		DamageBoxComponent = nullptr;
	}
}

void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnit::Select()
{
	SelectionSpriteComponent->SetVisibility(true);
}

void AUnit::Unselect()
{
	SelectionSpriteComponent->SetVisibility(false);
}

void AUnit::ClearTimers(AActor* Actor, EEndPlayReason::Type Reason)
{
	if (GetWorld())
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
		}
		if (GetWorld()->GetTimerManager().IsTimerActive(AudioTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(AudioTimer);
		}
	}
}

const FUnitInfo& AUnit::GetUnitData() const
{
	return *UnitData;
}

void AUnit::StartAttack(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AEntity* Entity = Cast<AEntity>(OtherActor);
	if (Entity && !Attacked.Contains(Entity))
	{
		Attacked.Add(Entity);
		if (!GetWorld()->GetTimerManager().IsTimerActive(AttackTimer))
		{
			Attack();
			GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AUnit::Attack, GameConstants::UnitDamageInterval, true);
		}
	}
}

void AUnit::StopAttack(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index)
{
	AEntity* Entity = Cast<AEntity>(OtherActor);
	if (Entity)
	{
		Attacked.Remove(Entity);
		if (Attacked.Num() == 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
		}
	}
}

void AUnit::Attack()
{
	for (AEntity* i : Attacked)
	{
		// Direction is zero, because unit cannot push entity (direction is not important)
		i->Damage(UnitData->Damage, UnitData->DamageType, 0.f, this);
	}
}
