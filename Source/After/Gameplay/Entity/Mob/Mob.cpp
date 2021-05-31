    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Mob.cpp                     //
////////////////////////////////////////

#include "Mob.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

#include "../../LogGameplay.h"
#include "../../../Data/Database/Database.h"
#include "../../../GameConstants.h"
#include "../../../AfterGameModeBase.h"
#include "../Controller/MobController.h"
#include "../../Item/ThrownItem.h"
#include "../../Item/Item.h"

AMob::AMob()
{
	// Здесь был Шарик
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AMobController::StaticClass();

	ViewComponent = CreateDefaultSubobject<USphereComponent>(TEXT("View Sphere"));
	ViewComponent->SetupAttachment(GetRootComponent());
	ViewComponent->SetCollisionProfileName(TEXT("TriggerArea"));
	ViewComponent->SetSphereRadius(GameConstants::TileSize.X);

	PursueComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Pursue Sphere"));
	PursueComponent->SetupAttachment(GetRootComponent());
	PursueComponent->SetCollisionProfileName(TEXT("TriggerArea"));
	PursueComponent->SetSphereRadius(2 * GameConstants::TileSize.X);

	CheckDangerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Check Danger Box"));
	CheckDangerBoxComponent->SetupAttachment(GetRootComponent());
	CheckDangerBoxComponent->SetCollisionProfileName(TEXT("TriggerArea"));
	CheckDangerBoxComponent->SetBoxExtent(GameConstants::TileSize + GameConstants::DamageBoxDelta);
}

void AMob::BeginPlay()
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
	MobData = &Database->GetMobData(Id);

	// Set controller
	AMobController* MobController = Cast<AMobController>(GetController());
	if (!MobController)
	{
		UE_LOG(LogGameplay, Error, TEXT("Mob (%s) doesn't have mob controller"), *Id.ToString());
	}
	else
	{
		MobController->MoveX.BindUObject(this, &AMob::SetMoveX);
		MobController->MoveY.BindUObject(this, &AMob::SetMoveY);
		MobController->StartRun.BindUObject(this, &AMob::StartRun);
		MobController->StopRun.BindUObject(this, &AMob::StopRun);
		MobController->Attack.BindUObject(this, &AMob::MeleeAttack);

		// Тут быў Шарик

		DamageDelegate.BindUObject(MobController, &AMobController::Damage);
		BeginDangerDelegate.BindUObject(MobController, &AMobController::BeginDanger);
		EndDangerDelegate.BindUObject(MobController, &AMobController::EndDanger);
		BeginViewDelegate.BindUObject(MobController, &AMobController::BeginView);
		EndPursueDelegate.BindUObject(MobController, &AMobController::EndPursue);

		MobController->SetupInput();
	}

	ViewComponent->SetSphereRadius(MobData->ViewRadius);
	ViewComponent->OnComponentBeginOverlap.AddDynamic(this, &AMob::BeginView);
	PursueComponent->SetSphereRadius(MobData->PursueRadius);
	PursueComponent->OnComponentEndOverlap.AddDynamic(this, &AMob::EndPursue);
	CheckDangerBoxComponent->SetBoxExtent(GameConstants::TileSize * FVector(EntityData->Size, 0.f) + EntityData->RunSpeed * GameConstants::MobUpdateDirectionTime);
	CheckDangerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMob::BeginDanger);
	CheckDangerBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMob::EndDanger);
}

void AMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FMobInfo& AMob::GetMobData() const
{
	return *MobData;
}

void AMob::Damage(float Value, FDamageType Type, float Direction, AActor* FromWho, float Push)
{
	Super::Damage(Value, Type, Direction, FromWho, Push);
	// Sharik was here
	DamageDelegate.ExecuteIfBound(Direction, FromWho);
}

void AMob::BeginDanger(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
	bool bFromSweep, const FHitResult& SweepResult)
{
	BeginDangerDelegate.ExecuteIfBound(OtherActor);
}

void AMob::EndDanger(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index)
{
	EndDangerDelegate.ExecuteIfBound(OtherActor);
}

void AMob::BeginView(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
	bool bFromSweep, const FHitResult& SweepResult)
{
	BeginViewDelegate.ExecuteIfBound(OtherActor);
}

void AMob::EndPursue(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index)
{
	EndPursueDelegate.ExecuteIfBound(OtherActor);
}

void AMob::DeathDrop()
{
	// Charik était là
	for (const FItemDrop& i : MobData->Drop)
	{
		static int SpawnPosition = 0;
		int Count;
		if ((i.Chance >= 1.f || FMath::RandRange(0.f, 1.f) < i.Chance) && (Count = FMath::RandRange(i.Min, i.Max)) > 0)
		{
			AThrownItem* Drop = GetWorld()->SpawnActor<AThrownItem>(GAME_MODE->GetDatabase()->GetExtraData().ThrownItemClass.Get(), GetActorLocation() + FVector(EntityData->Size, 0.f) * GameConstants::TileSize * FMath::RandRange(-.5f, .5f), FRotator(0.f, 0.f, 0.f));
			Drop->SetItem(GetWorld()->SpawnActor<AItem>(GAME_MODE->GetDatabase()->GetItemData(i.Item).Class.Get()));
		}
	}
}
