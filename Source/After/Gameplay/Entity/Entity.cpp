    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Entity.cpp                  //
////////////////////////////////////////

#include "Entity.h"

#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/AudioComponent.h"

#include "../../Data/Database.h"
#include "../../AfterGameModeBase.h"

AEntity::AEntity() :
	bIsDead(false),
	Moving(0.f, 0.f),
	bIsRunning(false),
	CurrentStatus(FEntityStatus::Special),	// Must not be default for the first
	CurrentDirection(FDirection::B),		// call of SetFlipbook to work correct
	bIsFlipbookFixed(false)
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetBoxExtent(AAfterGameModeBase::TileSize);
	CollisionComponent->SetCollisionProfileName(FName("Entity"));

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	FlipbookComponent->SetupAttachment(GetRootComponent());

	SelectionSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Selection Sprite"));
	SelectionSpriteComponent->SetupAttachment(FlipbookComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->SetupAttachment(GetRootComponent());
}

void AEntity::BeginPlay()
{
	Super::BeginPlay();

	// Get game mode
	AAfterGameModeBase* GameMode = Cast<AAfterGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Auth game mode is not AAfterGameModeBase"));
	}

	// Todo: Add selection by Last

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	EntityData = &Database->GetEntityData(Id);

	Health = EntityData->MaxHealth;
	Oxygen = EntityData->MaxOxygen;
	Energy = EntityData->MaxEnergy;

	CollisionComponent->SetBoxExtent(AAfterGameModeBase::TileSize * FVector(EntityData->Size, 1.f));
	FlipbookComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	FlipbookComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	SelectionSpriteComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	SelectionSpriteComponent->SetVisibility(false);
	AudioComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	SetFlipbook(FDirection::F, FEntityStatus::Stay);

	if (!Database->GetExtraData().SelectionSprites.Contains(EntityData->Size) ||
		!Database->GetExtraData().SelectionSprites[EntityData->Size])
	{
		UE_LOG(LogTemp, Error, TEXT("Database doesn't contain selection sprite with size %dx%d"), EntityData->Size.X, EntityData->Size.Y);
	}
	else
	{
		SelectionSpriteComponent->SetSprite(Database->GetExtraData().SelectionSprites[EntityData->Size]);
	}

	AudioDelegate.BindLambda([this]()
	{
		PlaySound(FEntitySoundType::Passive);
		GetWorld()->GetTimerManager().SetTimer(AudioTimer, AudioDelegate, FMath::RandRange(EntityData->MinSoundPause, EntityData->MaxSoundPause), false);
	});
	GetWorld()->GetTimerManager().SetTimer(AudioTimer, AudioDelegate, FMath::RandRange(EntityData->MinSoundPause, EntityData->MaxSoundPause), false);

	GetWorld()->GetTimerManager().SetTimer(StatsTimer, this, &AEntity::CalculateStats, AAfterGameModeBase::CalcStatsInterval, true);
}

void AEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsDead && CurrentStatus != FEntityStatus::Stone && CurrentStatus != FEntityStatus::Web)
	{
		Move(DeltaTime);
	}
}

void AEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
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

void AEntity::Damage(float Value, FDamageType Type, const AActor* FromWho)
{
	Health -= Value * EntityData->DamageResist[Type];
	SetFlipbook(CurrentDirection, FEntityStatus::Damage);

	if (Health <= 0.f)
	{
		Health = 0.f;
		Death(Type, FromWho);
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

void AEntity::Select()
{
	SelectionSpriteComponent->SetVisibility(true);
}

void AEntity::Unselect()
{
	SelectionSpriteComponent->SetVisibility(false);
}

const FEntityInfo& AEntity::GetEntityData() const
{
	return *EntityData;
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

bool AEntity::MeleeAttack(AEntity* Target)
{
	SetFlipbook(CurrentDirection, FEntityStatus::MeleeAttack);

	if (FVector::Dist(Target->GetActorLocation(), GetActorLocation()) <= EntityData->AttackRadius)
	{
		Target->Damage(EntityData->Damage, EntityData->DamageType, this);
		return true;
	}
	else
	{
		return false;
	}
}

void AEntity::RangedAttack(FRotator Direction)
{
	// Todo
}

void AEntity::Death(FDamageType Type, const AActor* Murderer)
{
	SetFlipbook(CurrentDirection, FEntityStatus::Death);
	bIsDead = true;
}

void AEntity::DeathDrop()
{

}

void AEntity::CalculateStats()
{
	if (!bIsRunning || Moving.IsZero())
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

		// Hit handling
		FHitResult HitResult;
		AddActorLocalOffset(Offset, true, &HitResult); // Move
		if (HitResult.bBlockingHit)
		{
			Offset = Offset.ProjectOnTo(FVector(-HitResult.Normal.Y, HitResult.Normal.X, 0.f));
			AddActorLocalOffset(Offset, true);
		}

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
				SetFlipbook(CurrentDirection, FEntityStatus::Stay);
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
