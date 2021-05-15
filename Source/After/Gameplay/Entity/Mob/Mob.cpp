    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Mob.cpp                     //
////////////////////////////////////////

#include "Mob.h"

#include "Components/SphereComponent.h"

#include "../../LogGameplay.h"
#include "../../../Data/Database/Database.h"
#include "../../../GameConstants.h"
#include "../../../AfterGameModeBase.h"
#include "../Controller/MobController.h"

AMob::AMob()
{
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

		DamageDelegate.BindUObject(MobController, &AMobController::Damage);
		DangerDelegate.BindUObject(MobController, &AMobController::Danger);
		BeginViewDelegate.BindUObject(MobController, &AMobController::BeginView);
		EndPursueDelegate.BindUObject(MobController, &AMobController::EndPursue);

		MobController->SetupInput();
	}

	ViewComponent->SetSphereRadius(MobData->ViewRadius);
	ViewComponent->OnComponentBeginOverlap.AddDynamic(this, &AMob::BeginView);
	PursueComponent->SetSphereRadius(MobData->PursueRadius);
	PursueComponent->OnComponentEndOverlap.AddDynamic(this, &AMob::EndPursue);
}

void AMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FMobInfo& AMob::GetMobData() const
{
	return *MobData;
}

void AMob::Damage(float Value, FDamageType Type, float Direction, const AActor* FromWho, float Push)
{
	Super::Damage(Value, Type, Direction, FromWho, Push);

	UE_LOG(LogTemp, Log, TEXT("Damage from %s"), FromWho ? *FromWho->GetName() : TEXT("Unknown"));

	DamageDelegate.ExecuteIfBound(Direction, FromWho);
}

void AMob::Danger(const AUnit* Unit)
{
	DangerDelegate.ExecuteIfBound(Unit);
}

void AMob::BeginView(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AEntity* Entity = Cast<AEntity>(OtherActor);
	if (Entity)
	{
		BeginViewDelegate.ExecuteIfBound(Entity);
	}
}

void AMob::EndPursue(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 Index)
{
	AEntity* Entity = Cast<AEntity>(OtherActor);
	if (Entity)
	{
		EndPursueDelegate.ExecuteIfBound(Entity);
	}
}
