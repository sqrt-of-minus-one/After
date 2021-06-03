    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Entity.cpp                  //
////////////////////////////////////////

#include "Entity.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/AudioComponent.h"

#include "../LogGameplay.h"
#include "../../Data/Database/Database.h"
#include "Controller/LastController.h"
#include "../Item/Item.h"
#include "../../AfterGameModeBase.h"
#include "../../GameConstants.h"

AEntity::AEntity() :
	Moving(0.f, 0.f),
	PushMoving(0.f, 0.f),
	bIsRunning(false),
	LastAttackTime(0.f),
	LastAttackInterval(0.f),
	bIsDead(false),
	CurrentStatus(FEntityStatus::Special),  // Must not be default for the first
	CurrentDirection(FDirection::B),        // call of SetFlipbook to work correct
	bIsFlipbookFixed(false)
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetBoxExtent(GameConstants::TileSize);
	CollisionComponent->SetCollisionProfileName(FName("Entity"));

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));

	SelectionSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Selection Sprite"));
	SelectionSpriteComponent->SetupAttachment(FlipbookComponent);
	SelectionSpriteComponent->SetVisibility(false);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->SetupAttachment(GetRootComponent());
}

void AEntity::BeginPlay()
{
	Super::BeginPlay();

	OnEndPlay.AddDynamic(this, &AEntity::ClearTimers);

	// Get game mode
	AAfterGameModeBase* GameMode = GAME_MODE;
	if (!GameMode)
	{
		UE_LOG(LogGameplay, Fatal, TEXT("Auth game mode is not AAfterGameModeBase"));
	}

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	EntityData = &Database->GetEntityData(Id);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEntity::StartOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AEntity::StopOverlap);
	CollisionComponent->SetBoxExtent(GameConstants::TileSize * FVector(EntityData->Size, 1.f));
	SelectionSpriteComponent->SetWorldLocation(GetActorLocation());
	AudioComponent->AttenuationSettings = Database->GetExtraData().SoundAttenuation;

	ALastController* LastController = Cast<ALastController>(GetWorld()->GetFirstPlayerController());
	if (LastController)
	{
		if (EntityData->bSelectable)
		{
			OnBeginCursorOver.AddDynamic(LastController, &ALastController::Select);
			OnEndCursorOver.AddDynamic(LastController, &ALastController::Unselect);

			if (!Database->GetExtraData().SelectionSprites.Contains(EntityData->Size) ||
				!Database->GetExtraData().SelectionSprites[EntityData->Size])
			{
				UE_LOG(LogGameplay, Error, TEXT("Database doesn't contain selection sprite with size %dx%d"), EntityData->Size.X, EntityData->Size.Y);
			}
			else
			{
				SelectionSpriteComponent->SetSprite(Database->GetExtraData().SelectionSprites[EntityData->Size]);
			}
		}
		else
		{
			// We don't need a sprite component if the entity is not selectable
			SelectionSpriteComponent->DestroyComponent();
			SelectionSpriteComponent = nullptr;
		}
	}
	else
	{
		UE_LOG(LogGameplay, Error, TEXT("Couldn't find Last Controller"));
	}

	Health = EntityData->MaxHealth;
	Oxygen = EntityData->MaxOxygen;
	Energy = EntityData->MaxEnergy;

	SetFlipbook(FDirection::F, FEntityStatus::Stay);

	if (EntityData->Sounds.Sounds[FEntitySoundType::Passive].Sounds.Num() != 0)
	{
		AudioDelegate.BindLambda([this]()
		{
			PlaySound(FEntitySoundType::Passive);
			GetWorld()->GetTimerManager().SetTimer(AudioTimer, AudioDelegate, FMath::RandRange(EntityData->MinSoundPause, EntityData->MaxSoundPause), false);
		});
		GetWorld()->GetTimerManager().SetTimer(AudioTimer, AudioDelegate, FMath::RandRange(EntityData->MinSoundPause, EntityData->MaxSoundPause), false);
	}

	GetWorld()->GetTimerManager().SetTimer(StatsTimer, this, &AEntity::CalculateStats, GameConstants::CalcStatsInterval, true);
}

void AEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PushMoving.IsZero())
	{
		AddOffset(FVector(PushMoving, 0.f));
		PushMoving *= GameConstants::EntityPushDecrement;
	}

	FVector2D OverlapOffset(0.f, 0.f);
	for (const AEntity* i : OverlappingEntities)
	{
		FVector2D Delta(GetActorLocation() - i->GetActorLocation());
		if (!Delta.IsNearlyZero(1.f))
		{
			OverlapOffset += GameConstants::EntityOverlapOffsetMultiplier * Delta / (Delta.X * Delta.X + Delta.Y * Delta.Y);
		}
		else
		{
			OverlapOffset += GameConstants::EntityOverlapOffsetMultiplier * FVector2D(FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f));
		}
	}
	if (!OverlapOffset.IsZero())
	{
		AddOffset(FVector(OverlapOffset, 0.f));
	}

	if (!bIsDead && CurrentStatus != FEntityStatus::Stone && CurrentStatus != FEntityStatus::Web)
	{
		Move(DeltaTime);
	}
}

const FEntityInfo& AEntity::GetEntityData() const
{
	return *EntityData;
}

const FGameplayTag& AEntity::GetId() const
{
	return Id;
}

float AEntity::GetHealth() const
{
	return Health;
}

float AEntity::GetOxygen() const
{
	return Oxygen;
}

float AEntity::GetEnergy() const
{
	return Energy;
}

void AEntity::Damage(float Value, FDamageType Type, float Direction, AActor* FromWho, float Push)
{
	Health -= Value * EntityData->DamageResist[Type];
	PushMoving += FVector2D(Push * FMath::Cos(Direction), Push * FMath::Sin(Direction));

	if (Health <= 0.f)
	{
		Health = 0.f;
		Death(Type, FromWho);
	}
	else
	{
		SetFlipbook(CurrentDirection, FEntityStatus::Damage);
		PlaySound(FEntitySoundType::Damage);
	}
}

void AEntity::Stone(float Duration)
{
	SetFlipbook(CurrentDirection, FEntityStatus::Stone, Duration);
}

void AEntity::Web(float Duration)
{
	SetFlipbook(CurrentDirection, FEntityStatus::Web, Duration);
}

bool AEntity::IsDead()
{
	return bIsDead;
}

void AEntity::Select()
{
	SelectionSpriteComponent->SetVisibility(true);
}

void AEntity::Unselect()
{
	SelectionSpriteComponent->SetVisibility(false);
}

void AEntity::ClearTimers(AActor* Actor, EEndPlayReason::Type Reason)
{
	if (GetWorld())
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();

		if (TimerManager.IsTimerActive(PoisonTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(PoisonTimer);
		}
		if (TimerManager.IsTimerActive(RadiationTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(RadiationTimer);
		}
		if (TimerManager.IsTimerActive(StatsTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(StatsTimer);
		}
		if (TimerManager.IsTimerActive(FixedFlipbookTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(FixedFlipbookTimer);
		}
		if (TimerManager.IsTimerActive(AudioTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(AudioTimer);
		}
	}
}

void AEntity::Move(float DeltaTime)
{
	const float Sqrt_2 = 1.41421f;

	FVector Offset(Moving, 0.f);
	if (!Offset.IsZero())
	{
		if (bIsRunning && Energy <= 0.f)
		{
			Energy = 0.f;
			StopRun();
		}
		Offset *= (bIsRunning ? EntityData->RunSpeed : EntityData->WalkSpeed) * DeltaTime;
		if (Moving.X != 0.f && Moving.Y != 0.f) // Diagonal movement
		{
			Offset /= Sqrt_2;
		}

		AddOffset(Offset);

		FDirection RequiredDirection;
		if (Moving.Y < 0)
		{
			RequiredDirection = FDirection::F;
		}
		else if (Moving.Y > 0)
		{
			RequiredDirection = FDirection::B;
		}
		else if (Moving.X < 0)
		{
			RequiredDirection = FDirection::L;
		}
		else
		{
			RequiredDirection = FDirection::R;
		}
		FEntityStatus RequiredStatus = bIsRunning ? FEntityStatus::Run : FEntityStatus::Walk;
		SetFlipbook(RequiredDirection, RequiredStatus);

		if (bIsRunning)
		{
			Energy -= EntityData->EnergySpeed * DeltaTime;
		}
	}
	else
	{
		SetFlipbook(CurrentDirection, FEntityStatus::Stay);
	}
}

void AEntity::AddOffset(FVector Offset)
{
	Offset.Z = 0.f;
	FHitResult HitResult;
	AddActorLocalOffset(Offset, true, &HitResult); // Move
	if (HitResult.bBlockingHit)
	{
		Offset = Offset.ProjectOnTo(FVector(-HitResult.Normal.Y, HitResult.Normal.X, 0.f));
		AddActorLocalOffset(Offset, true);
	}
}

void AEntity::SetMoveX(float Value)
{
	Moving.X = FMath::Clamp(Value, -1.f, 1.f);
}

void AEntity::SetMoveY(float Value)
{
	Moving.Y = FMath::Clamp(Value, -1.f, 1.f);
}

void AEntity::StartRun()
{
	if (Energy > 0.f)
	{
		bIsRunning = true;
	}
}

void AEntity::StopRun()
{
	bIsRunning = false;
}

void AEntity::StartOverlap(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AEntity* Entity = Cast<AEntity>(OtherActor);
	if (Entity && OtherComponent->GetCollisionProfileName() == CollisionComponent->GetCollisionProfileName() && !OverlappingEntities.Contains(Entity))
	{
		OverlappingEntities.Add(Entity);
	}
}

void AEntity::StopOverlap(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index)
{
	AEntity* Entity = Cast<AEntity>(OtherActor);
	if (Entity && OverlappingEntities.Contains(Entity))
	{
		OverlappingEntities.Remove(Entity);
	}
}

bool AEntity::MeleeAttack(AEntity* Target, bool bCanMiss, AItem* Weapon)
{
	float CurrentTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
	if (Weapon && (Weapon->GetItemData().Damage == 0.f || Weapon->GetItemData().AttackRadius <= 0.f))
	{
		Weapon = nullptr;
	}
	if (CurrentTime - LastAttackTime > LastAttackInterval && Target != this && !bIsDead)
	{
		if (FVector::DistSquared(Target->GetActorLocation(), GetActorLocation()) <=
			FMath::Square(Weapon ? Weapon->GetItemData().AttackRadius : EntityData->AttackRadius))
		{
			SetFlipbook(CurrentDirection, FEntityStatus::MeleeAttack);
			PlaySound(FEntitySoundType::Attack);
			LastAttackTime = CurrentTime;

			FVector2D Direction = static_cast<FVector2D>(Target->GetActorLocation() - GetActorLocation());
			if (Weapon)
			{
				Target->Damage(Weapon->GetItemData().Damage, Weapon->GetItemData().DamageType, FMath::Atan2(Direction.Y, Direction.X), this, Weapon->GetItemData().Push);
				LastAttackInterval = Weapon->GetItemData().AttackInterval;
			}
			else
			{
				Target->Damage(EntityData->Damage, EntityData->DamageType, FMath::Atan2(Direction.Y, Direction.X), this, EntityData->Push);
				LastAttackInterval = EntityData->AttackInterval;
			}
			return true;
		}
		else if (bCanMiss)
		{
			SetFlipbook(CurrentDirection, FEntityStatus::MeleeAttack);
			PlaySound(FEntitySoundType::Attack);
			LastAttackTime = CurrentTime;
			LastAttackInterval = Weapon ? Weapon->GetItemData().AttackInterval : EntityData->AttackInterval;
		}
	}
	return false;
}

void AEntity::RangedAttack(FRotator Direction)
{
	// Todo
}

void AEntity::Death(FDamageType Type, const AActor* Murderer)
{
	SetFlipbook(CurrentDirection, FEntityStatus::Death);
	PlaySound(FEntitySoundType::Death);
	bIsDead = true;
	DeathDrop();
}

void AEntity::Disappear()
{
	GetWorld()->DestroyActor(this);
}

void AEntity::DeathDrop()
{

}

void AEntity::CalculateStats()
{
	if (Energy < EntityData->MaxEnergy && (!bIsRunning || Moving.IsZero()))
	{
		Energy = FMath::Clamp(Energy + EntityData->EnergyRegenerationSpeed, 0.f, EntityData->MaxEnergy);
	}

	/* Todo
	if ()
	{
		Oxygen = FMath::Clamp(Oxygen + EntityData->OxygenRegenerationSpeed, 0.f, EntityData->MaxOxygen);
	}
	*/
}

void AEntity::SetFlipbook(FDirection Direction, FEntityStatus Status, float Time)
{
	// Whether flipbook have to be fixed with this status
	bool bIsStatusFixing =
		Status != FEntityStatus::Stay && Status != FEntityStatus::Walk && Status != FEntityStatus::Run &&
		Status != FEntityStatus::SwimStay && Status != FEntityStatus::SwimMove;

	if (!bIsFlipbookFixed || bIsStatusFixing)
	{
		if (CurrentStatus != Status || bIsStatusFixing)
		{
			CurrentDirection = Direction;
			CurrentStatus = Status;
			FlipbookComponent->SetFlipbook(EntityData->Flipbooks[Status].Flipbooks[Direction]);
		}

		if (bIsStatusFixing)
		{
			bIsFlipbookFixed = true;
			FTimerDelegate FixedFlipbookDelegate;
			FixedFlipbookDelegate.BindLambda([this]()
			{
				// "Unfix" flipbook
				bIsFlipbookFixed = false;
				if (bIsDead)
				{
					Disappear();
				}
				else
				{
					SetFlipbook(CurrentDirection, FEntityStatus::Stay);
				}
			});
			GetWorld()->GetTimerManager().SetTimer(FixedFlipbookTimer, FixedFlipbookDelegate,
				(Status == FEntityStatus::Stone || Status == FEntityStatus::Web) ? Time : FlipbookComponent->GetFlipbookLength(), false);
		}
	}
	
	if (CurrentDirection != Direction)
	{
		CurrentDirection = Direction;
		FlipbookComponent->SetFlipbook(EntityData->Flipbooks[CurrentStatus].Flipbooks[Direction]);
	}
}

void AEntity::PlaySound(FEntitySoundType Sound)
{
	int Size = EntityData->Sounds.Sounds[Sound].Sounds.Num();
	if (Size != 0)
	{
		AudioComponent->SetSound(EntityData->Sounds.Sounds[Sound].Sounds[FMath::RandRange(0, Size - 1)]);
		AudioComponent->Play();
	}
}
