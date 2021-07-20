    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Unit.cpp                    //
////////////////////////////////////////

#include "Unit.h"

#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"

#include "../../Data/Database/Database.h"
#include "../LogGameplay.h"
#include "../Entity/Controller/LastController.h"
#include "../../AfterGameModeBase.h"
#include "../Entity/Entity.h"
#include "../Entity/Mob/Mob.h"
#include "../../GameConstants.h"

AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetBoxExtent(GameConstants::TileSize);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));

	SelectionSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Selection Sprite"));
	SelectionSpriteComponent->SetupAttachment(FlipbookComponent);
	SelectionSpriteComponent->SetVisibility(false);

	DamageBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Box"));
	DamageBoxComponent->SetupAttachment(GetRootComponent());
	DamageBoxComponent->SetCollisionProfileName(TEXT("TriggerArea"));
	DamageBoxComponent->SetBoxExtent(GameConstants::TileSize + GameConstants::DamageBoxDelta);
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();

	// Get game mode
	AAfterGameModeBase* GameMode = GAME_MODE;
	if (!GameMode)
	{
		UE_LOG(LogGameplay, Fatal, TEXT("Auth game mode is not AAfterGameModeBase"));
	}

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	UnitData = &Database->GetUnitData(Id);

	//SelectionSpriteComponent->SetWorldLocation(GetActorLocation());

	ALastController* LastController = Cast<ALastController>(GetWorld()->GetFirstPlayerController());
	if (LastController)
	{
		if (UnitData->bSelectable)
		{
			OnBeginCursorOver.AddDynamic(LastController, &ALastController::Select);
			OnBeginCursorOver.AddDynamic(this, &AUnit::Select);
			OnEndCursorOver.AddDynamic(LastController, &ALastController::Unselect);
			OnEndCursorOver.AddDynamic(this, &AUnit::Unselect);
		}
		else
		{
			// We don't need a selection sprite if the unit is not selectable
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
		// We don't need a damage box if the unit does not deal damage
		DamageBoxComponent->DestroyComponent();
		DamageBoxComponent = nullptr;
	}
}

void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FUnitInfo& AUnit::GetUnitData() const
{
	return *UnitData;
}

const FGameplayTag& AUnit::GetId() const
{
	return Id;
}

void AUnit::Select(AActor* Actor)
{
	SelectionSpriteComponent->SetVisibility(true);
}

void AUnit::Unselect(AActor* Actor)
{
	SelectionSpriteComponent->SetVisibility(false);
}

void AUnit::StartAttack(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AEntity* Entity = Cast<AEntity>(OtherActor);
	if (IsValid(Entity) && !Attacked.Contains(Entity))
	{
		Attacked.AddTail(Entity);
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
		Attacked.RemoveNode(Entity);
		if (Attacked.Num() == 0)
		{
			// Stop attack if there is no attacked entity
			GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
		}
	}
}

void AUnit::Attack()
{
	for (AEntity* i : Attacked)
	{
		if (IsValid(i))
		{
			// Direction is not important (and is zero), because unit cannot push entity
			i->Damage(UnitData->Damage, UnitData->DamageType, 0.f, this);
		}
	}
}
