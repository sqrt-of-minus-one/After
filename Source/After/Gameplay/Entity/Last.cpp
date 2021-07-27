    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Last.cpp                    //
////////////////////////////////////////

#include "Last.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperFlipbookComponent.h"

#include "../LogGameplay.h"
#include "../../Data/Database/Database.h"
#include "../../Data/Lang/LangManager.h"
#include "Controller/LastController.h"
#include "../../AfterGameModeBase.h"
#include "../../Gui/WidgetInitializer.h"
#include "../../GameConstants.h"
#include "../Unit/SolidUnit/SolidUnit.h"
#include "../../Components/Inventory/PlayerInventoryComponent.h"

ALast::ALast() :
	DestroyerId(-1),
	DestroyedUnit(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeRotation(FRotator(-90.f, -90.f, 0.f));
	SpringArmComponent->TargetArmLength = GameConstants::PlayerSpringArmLength;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	InventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("Inventory"));
}

void ALast::BeginPlay()
{
	Super::BeginPlay();

	// Get database
	const UDatabase* Database = GAME_MODE->GetDatabase();
	LastData = &Database->GetLastData(Id);

	Satiety = LastData->MaxSatiety;

	InventoryComponent->Init(LastData->InventorySize, LastData->HotbarSize);

	AWidgetInitializer* WidgetInitializer = GAME_MODE->GetWidgetInitializer();
	if (IsValid(WidgetInitializer))
	{
		WidgetInitializer->DisplayMainWidget(this);
	}
}

void ALast::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// Debug output
	const ALangManager* LangManager = GAME_MODE->GetLangManager();

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("%s: %f"), *LangManager->GetString(FName("stats.oxygen")), Oxygen));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("%s: %f"), *LangManager->GetString(FName("stats.energy")), Energy));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Magenta, FString::Printf(TEXT("%s: %f"), *LangManager->GetString(FName("stats.satiety")), Satiety));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("%s: %f"), *LangManager->GetString(FName("stats.health")), Health));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("FPS: %.2f (%.2f ms)"), 1 / DeltaTime, DeltaTime * 1000));
// End debug output

	// Todo: Use events (SolidUnit)
	if (DestroyedUnit)
	{
		if (!IsValid(DestroyedUnit))
		{
			StopBreak();
		}
		else if (DestroyerId >= 0)
		{
			if (FVector::DistSquared(DestroyedUnit->GetActorLocation(), GetActorLocation()) > FMath::Square(EntityData->AttackRadius))
			{
				DestroyedUnit->StopBreaking(DestroyerId);
				DestroyerId = -1;
			}
		}
		else
		{
			if (FVector::DistSquared(DestroyedUnit->GetActorLocation(), GetActorLocation()) <= FMath::Square(EntityData->AttackRadius))
			{
				DestroyerId = DestroyedUnit->StartBreaking(ItemForBreaking);
			}
		}
	}
}

void ALast::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ALastController* LastController = Cast<ALastController>(NewController);
	if (IsValid(LastController))
	{
		LastController->MoveX.BindUObject(this, &ALast::SetMoveX);
		LastController->MoveY.BindUObject(this, &ALast::SetMoveY);
		LastController->ZoomIn.BindUObject(this, &ALast::ZoomIn);
		LastController->ZoomOut.BindUObject(this, &ALast::ZoomOut);
		LastController->StartRun.BindUObject(this, &ALast::StartRun);
		LastController->StopRun.BindUObject(this, &ALast::StopRun);
		LastController->Attack.BindUObject(this, &ALast::MeleeAttack);
		LastController->StartBreak.BindUObject(this, &ALast::StartBreak);
		LastController->StopBreak.BindUObject(this, &ALast::StopBreak);

		LastController->OnItemChanged.AddDynamic(this, &ALast::SetItem);
	}
	else
	{
		UE_LOG(LogGameplay, Error, TEXT("Last (%s) doesn't have last controller"), *Id.ToString());
	}
}

const FLastInfo& ALast::GetLastData() const
{
	return *LastData;
}

float ALast::GetSatiety() const
{
	return Satiety;
}

UPlayerInventoryComponent* ALast::GetInventory()
{
	return InventoryComponent;
}

void ALast::Weak()
{
	// Todo
}

void ALast::ZoomIn()
{
	SpringArmComponent->TargetArmLength = FMath::Clamp(
		SpringArmComponent->TargetArmLength / GameConstants::ZoomStep,
		GameConstants::MinPlayerSpringArmLength, GameConstants::MaxPlayerSpringArmLength);
}

void ALast::ZoomOut()
{
	SpringArmComponent->TargetArmLength = FMath::Clamp(
		SpringArmComponent->TargetArmLength * GameConstants::ZoomStep,
		GameConstants::MinPlayerSpringArmLength, GameConstants::MaxPlayerSpringArmLength);
}

void ALast::SetItem(AItem* Item)
{
	ItemForBreaking = Item;
	if (IsValid(DestroyedUnit) && DestroyerId >= 0)
	{
		DestroyedUnit->SwitchItem(DestroyerId, ItemForBreaking);
	}
}

void ALast::StartBreak(ASolidUnit* Target, AItem* Item)
{
	StopBreak();
	DestroyedUnit = Target;
	ItemForBreaking = Item;
}

void ALast::StopBreak()
{
	if (IsValid(DestroyedUnit) && DestroyerId >= 0)
	{
		DestroyedUnit->StopBreaking(DestroyerId);
	}

	DestroyedUnit = nullptr;
	DestroyerId = -1;
}

void ALast::Disappear()
{
	FlipbookComponent->SetPlaybackPosition(FlipbookComponent->GetFlipbookLength(), false);
	FlipbookComponent->Stop();
}

void ALast::CalculateStats()
{
	Super::CalculateStats();

	Satiety = FMath::Clamp(Satiety - LastData->SatietySpeed, 0.f, LastData->MaxSatiety);
	OnSatietyChanged.Broadcast(Satiety);
}
