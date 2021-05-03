    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Last.cpp                    //
////////////////////////////////////////

#include "Last.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "../LogGameplay.h"
#include "../../Data/Database.h"
#include "../../AfterGameModeBase.h"
#include "Controller/LastController.h"

ALast::ALast()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeRotation(FRotator(-90.f, -90.f, 0.f));
	SpringArmComponent->TargetArmLength = AAfterGameModeBase::PlayerSpringArmLength;
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
	AAfterGameModeBase* GameMode = Cast<AAfterGameModeBase>(GetWorld()->GetAuthGameMode());
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

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("Oxygen: %f"), Oxygen));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Energy: %f"), Energy));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("Satiety: %f"), Satiety));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("Health: %f"), Health));
}

void ALast::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ALast::GetSatiety() const
{
	return Satiety;
}

void ALast::Weak()
{
	// Todo
}

const FLastInfo& ALast::GetLastData() const
{
	return *LastData;
}

void ALast::ZoomIn()
{
	SpringArmComponent->TargetArmLength = FMath::Clamp(
		SpringArmComponent->TargetArmLength / AAfterGameModeBase::ZoomStep,
		AAfterGameModeBase::MinPlayerSpringArmLength, AAfterGameModeBase::MaxPlayerSpringArmLength);
}

void ALast::ZoomOut()
{
	SpringArmComponent->TargetArmLength = FMath::Clamp(
		SpringArmComponent->TargetArmLength * AAfterGameModeBase::ZoomStep,
		AAfterGameModeBase::MinPlayerSpringArmLength, AAfterGameModeBase::MaxPlayerSpringArmLength);
}

void ALast::CalculateStats()
{
	Super::CalculateStats();

	Satiety = FMath::Clamp(Satiety - LastData->SatietySpeed, 0.f, LastData->MaxSatiety);
}
