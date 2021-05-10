    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Last.cpp                    //
////////////////////////////////////////

#include "Last.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "../LogGameplay.h"
#include "../../Data/Database/Database.h"
#include "../../Data/Lang/LangManager.h"
#include "Controller/LastController.h"
#include "../../AfterGameModeBase.h"
#include "../../GameConstants.h"

ALast::ALast()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeRotation(FRotator(-90.f, -90.f, 0.f));
	SpringArmComponent->TargetArmLength = GameConstants::PlayerSpringArmLength;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ALast::BeginPlay()
{
	Super::BeginPlay();

	ALastController* LastController = Cast<ALastController>(GetController());
	if (!LastController)
	{
		UE_LOG(LogGameplay, Fatal, TEXT("Last (%s) doesn't have last controller"), *Id.ToString());
	}
	LastController->MoveX.BindUObject(this, &ALast::SetMoveX);
	LastController->MoveY.BindUObject(this, &ALast::SetMoveY);
	LastController->ZoomIn.BindUObject(this, &ALast::ZoomIn);
	LastController->ZoomOut.BindUObject(this, &ALast::ZoomOut);
	LastController->StartRun.BindUObject(this, &ALast::StartRun);
	LastController->StopRun.BindUObject(this, &ALast::StopRun);
	LastController->Attack.BindUObject(this, &ALast::MeleeAttack);
	LastController->SetupInput();

	// Get game mode
	AAfterGameModeBase* GameMode = GAME_MODE;
	if (!GameMode)
	{
		UE_LOG(LogGameplay, Fatal, TEXT("Auth game mode is not AAfterGameModeBase"));
	}

	// Get database
	const UDatabase* Database = GameMode->GetDatabase();
	LastData = &Database->GetLastData(Id);

	Satiety = LastData->MaxSatiety;
}

void ALast::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const ALangManager* LangManager = GAME_MODE->GetLangManager();

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("%s: %f"), *LangManager->GetString(FName("stats.oxygen")), Oxygen));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("%s: %f"), *LangManager->GetString(FName("stats.energy")), Energy));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Magenta, FString::Printf(TEXT("%s: %f"), *LangManager->GetString(FName("stats.satiety")), Satiety));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("%s: %f"), *LangManager->GetString(FName("stats.health")), Health));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("\nFPS: %.2f (%.2f ms)"), 1 / DeltaTime, DeltaTime * 1000));
}

const FLastInfo& ALast::GetLastData() const
{
	return *LastData;
}

float ALast::GetSatiety() const
{
	return Satiety;
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

void ALast::CalculateStats()
{
	Super::CalculateStats();

	Satiety = FMath::Clamp(Satiety - LastData->SatietySpeed, 0.f, LastData->MaxSatiety);
}
