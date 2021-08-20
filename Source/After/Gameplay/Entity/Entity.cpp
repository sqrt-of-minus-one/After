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
#include "Containers/List.h"

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
	bIsFlipbookFixed(false),
	CurrentStatus(FEntityStatus::Walk),     // Must not be default for the first
	CurrentDirection(FDirection::B)         // call of SetFlipbook to work correct
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetBoxExtent(GameConstants::TileSize - GameConstants::EntitySizeDelta);
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
	CollisionComponent->SetBoxExtent(GameConstants::TileSize * FVector(EntityData->Size, 1.f) - GameConstants::EntitySizeDelta);
	AudioComponent->AttenuationSettings = Database->GetExtraData().SoundAttenuation;

	ALastController* LastController = Cast<ALastController>(GetWorld()->GetFirstPlayerController());
	if (LastController)
	{
		if (EntityData->bSelectable)
		{
			OnBeginCursorOver.AddDynamic(LastController, &ALastController::Select);
			OnBeginCursorOver.AddDynamic(this, &AEntity::Select);
			OnEndCursorOver.AddDynamic(LastController, &ALastController::Unselect);
			OnEndCursorOver.AddDynamic(this, &AEntity::Unselect);
			OnDestroyed.AddDynamic(LastController, &ALastController::Unselect);
			OnDestroyed.AddDynamic(this, &AEntity::Unselect);

			if (Database->GetExtraData().SelectionSprites.Contains(EntityData->Size) &&
				Database->GetExtraData().SelectionSprites[EntityData->Size])
			{
				SelectionSpriteComponent->SetSprite(Database->GetExtraData().SelectionSprites[EntityData->Size]);
			}
			else
			{
				UE_LOG(LogGameplay, Error, TEXT("Database doesn't contain selection sprite with size %dx%d"), EntityData->Size.X, EntityData->Size.Y);
			}
		}
		else
		{
			// We don't need a selection sprite component if the entity is not selectable
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
		FTimerHandle AudioTimer;
		GetWorld()->GetTimerManager().SetTimer(AudioTimer, this, &AEntity::PlayPassiveSound,
			FMath::RandRange(EntityData->MinSoundPause, EntityData->MaxSoundPause), false);
	}

	GameMode->OnGameTick.AddDynamic(this, &AEntity::CalculateStats);
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
	float S, C;
	FMath::SinCos(&S, &C, Direction);
	PushMoving += FVector2D(Push * C, Push * S);

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

	OnDamage.Broadcast(Value, Type, Direction, FromWho, Push);
	OnHealthChanged.Broadcast(Health);
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

void AEntity::Select(AActor* Actor)
{
	SelectionSpriteComponent->SetVisibility(true);
}

void AEntity::Unselect(AActor* Actor)
{
	SelectionSpriteComponent->SetVisibility(false);
}

void AEntity::Move(float DeltaTime)
{
	FVector Offset(Moving, 0.f);
	if (!Offset.IsZero())
	{
		Offset.Normalize();
		if (bIsRunning && Energy <= 0.f)
		{
			Energy = 0.f;
			OnEnergyChanged.Broadcast(Energy);
			StopRun();
		}
		Offset *= (bIsRunning ? EntityData->RunSpeed : EntityData->WalkSpeed) * DeltaTime;

		AddOffset(Offset);

		FDirection RequiredDirection;

		float Tg = Offset.Y / Offset.X;
		if (Offset.Y > Offset.X)
		{
			RequiredDirection = Offset.Y > -Offset.X ? FDirection::B : FDirection::L;
		}
		else
		{
			RequiredDirection = Offset.Y > -Offset.X ? FDirection::R : FDirection::F;
		}

		FEntityStatus RequiredStatus = bIsRunning ? FEntityStatus::Run : FEntityStatus::Walk;
		SetFlipbook(RequiredDirection, RequiredStatus);

		if (bIsRunning)
		{
			Energy -= EntityData->EnergySpeed * DeltaTime;
			OnEnergyChanged.Broadcast(Energy);
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
		OverlappingEntities.AddTail(Entity);
	}
}

void AEntity::StopOverlap(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index)
{
	AEntity* Entity = Cast<AEntity>(OtherActor);
	if (Entity)
	{
		OverlappingEntities.RemoveNode(Entity);
	}
}

bool AEntity::MeleeAttack(AEntity* Target, bool bCanMiss, AItem* Weapon)
{
	if (IsValid(Target))
	{
		// Save the time of attack
		float CurrentTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
		
		if (CurrentTime - LastAttackTime > LastAttackInterval && Target != this && !bIsDead)
		{
			// Items with zero damage or non-positive attack radius cannot be used as weapons
			if (IsValid(Weapon) && (Weapon->GetItemData().Damage == 0.f || Weapon->GetItemData().AttackRadius <= 0.f))
			{
				Weapon = nullptr;
			}

			bool bCanAttack = FVector::DistSquared(Target->GetActorLocation(), GetActorLocation()) <=
				FMath::Square(IsValid(Weapon) ? Weapon->GetItemData().AttackRadius : EntityData->AttackRadius);

			if (bCanAttack)
			{
				FVector2D Direction = static_cast<FVector2D>(Target->GetActorLocation() - GetActorLocation());
				if (IsValid(Weapon))
				{
					Target->Damage(Weapon->GetItemData().Damage, Weapon->GetItemData().DamageType, FMath::Atan2(Direction.Y, Direction.X), this, Weapon->GetItemData().Push);
					Weapon->DecreaseCondition(GameConstants::ItemConditionDecrease);
				}
				else
				{
					Target->Damage(EntityData->Damage, EntityData->DamageType, FMath::Atan2(Direction.Y, Direction.X), this, EntityData->Push);
				}
			}
			
			if (bCanAttack || bCanMiss)
			{
				SetFlipbook(CurrentDirection, FEntityStatus::MeleeAttack);
				PlaySound(FEntitySoundType::Attack);
				LastAttackTime = CurrentTime;
				LastAttackInterval = IsValid(Weapon) ? Weapon->GetItemData().AttackInterval : EntityData->AttackInterval;
			}

			return bCanAttack;
		}
	}
	return false;
}

void AEntity::RangedAttack(FRotator Direction)
{
	// Todo
}

void AEntity::Death(FDamageType Type, AActor* Murderer)
{
	GAME_MODE->OnGameTick.RemoveAll(this);
	SetFlipbook(CurrentDirection, FEntityStatus::Death);
	PlaySound(FEntitySoundType::Death);
	bIsDead = true;
	DeathDrop();
	OnDeath.Broadcast(Type, Murderer);
}

void AEntity::Disappear()
{
	GetWorld()->DestroyActor(this);
}

void AEntity::DeathDrop()
{
	// Pure
}

void AEntity::CalculateStats()
{
	if (Energy < EntityData->MaxEnergy && (!bIsRunning || Moving.IsZero()))
	{
		Energy = FMath::Clamp(Energy + EntityData->EnergyRegenerationSpeed * GameConstants::GameTickLength, 0.f, EntityData->MaxEnergy);
		OnEnergyChanged.Broadcast(Energy);
	}

	if (Oxygen < EntityData->MaxOxygen /* Todo: and if is not underwater */)
	{
		Oxygen = FMath::Clamp(Oxygen + EntityData->OxygenRegenerationSpeed * GameConstants::GameTickLength, 0.f, EntityData->MaxOxygen);
		OnOxygenChanged.Broadcast(Oxygen);
	}
	else if (Oxygen <= 0.f)
	{
		Damage(EntityData->OxygenDamage * GameConstants::GameTickLength, FDamageType::Suffocation, 0.f, nullptr, 0.f);
	}
}

void AEntity::SetFlipbook(FDirection Direction, FEntityStatus Status, float Time)
{
	// Whether flipbook have to be fixed with this status
	bool bIsStatusFixing =
		Status != FEntityStatus::Stay && Status != FEntityStatus::Walk && Status != FEntityStatus::Run &&
		Status != FEntityStatus::SwimStay && Status != FEntityStatus::SwimMove;

	// If we should update flipbook
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
			FTimerHandle FixedFlipbookTimer;
			GetWorld()->GetTimerManager().SetTimer(FixedFlipbookTimer, this, &AEntity::UnfixFlipbook,
				(Status == FEntityStatus::Stone || Status == FEntityStatus::Web) ? Time : FlipbookComponent->GetFlipbookLength(), false);
		}
	}
	
	if (CurrentDirection != Direction)
	{
		CurrentDirection = Direction;
		FlipbookComponent->SetFlipbook(EntityData->Flipbooks[CurrentStatus].Flipbooks[Direction]);
	}
}

void AEntity::UnfixFlipbook()
{
	bIsFlipbookFixed = false;
	if (bIsDead)
	{
		Disappear();
	}
	else
	{
		SetFlipbook(CurrentDirection, FEntityStatus::Stay);
	}
}

void AEntity::PlayPassiveSound()
{
	PlaySound(FEntitySoundType::Passive);
	FTimerHandle AudioTimer;
	GetWorld()->GetTimerManager().SetTimer(AudioTimer, this, &AEntity::PlayPassiveSound, FMath::RandRange(EntityData->MinSoundPause, EntityData->MaxSoundPause), false);
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
