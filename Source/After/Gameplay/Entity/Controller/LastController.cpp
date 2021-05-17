    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: LastController.cpp          //
////////////////////////////////////////

#include "LastController.h"

#include "GameFramework/GameUserSettings.h"

#include "../../../Data/Database/Database.h"
#include "../../../Data/Lang/LangManager.h"
#include "../../LogGameplay.h"
#include "../Entity.h"
#include "../../Unit/SolidUnit/SolidUnit.h"
#include "../../../AfterGameModeBase.h"
#include "GameplayTagContainer.h"
#include "../Mob/Animal.h"

ALastController::ALastController() :
	bIsBreaking(false)
{
	SetShowMouseCursor(true);
	bEnableMouseOverEvents = true;
}

void ALastController::BeginPlay()
{
	Super::BeginPlay();

	EntityPawn = Cast<AEntity>(GetPawn());
	if (!EntityPawn)
	{
		UE_LOG(LogGameplay, Fatal, TEXT("Last Controller: Pawn is not entity"));
	}
}

void ALastController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const ALangManager* LangManager = GAME_MODE->GetLangManager();

	AEntity* SelectedEntity = nullptr;
	AUnit* SelectedUnit = nullptr;
	if (SelectedEntity = Cast<AEntity>(Selected), SelectedEntity)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Orange, FString::Printf(TEXT("*          %s: %f"), *LangManager->GetString(FName("stats.health")), SelectedEntity->GetHealth()));

		FString EntityName = GAME_MODE->GetLangManager()->GetString(FName(SelectedEntity->GetId().ToString() + FString(".name")));

		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("%s: %s"), *LangManager->GetString(FName("tmp.selected")), *EntityName));
	}
	else if (SelectedUnit = Cast<AUnit>(Selected), SelectedUnit)
	{
		FString UnitName = GAME_MODE->GetLangManager()->GetString(FName(SelectedUnit->GetId().ToString() + FString(".name")));

		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("%s: %s"), *LangManager->GetString(FName("tmp.selected")), *UnitName));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("%s: None"), *LangManager->GetString(FName("tmp.selected"))));
	}
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("\n")));
}

void ALastController::Select(AActor* Actor)
{
	AEntity* Entity = Cast<AEntity>(Actor);
	AUnit* Unit = nullptr;
	bool bNew = false;
	AActor* Old = Selected;
	if (Entity)
	{
		Selected = Entity;
		Entity->Select();
		bNew = true;
	}
	else
	{
		Unit = Cast<AUnit>(Actor);
		if (Unit)
		{
			Selected = Unit;
			Unit->Select();
			bNew = true;
		}
	}

	if (bNew && Old)
	{
		Unselect(Old);
	}

	if (bIsBreaking)
	{
		ASolidUnit* SolidUnit = Cast<ASolidUnit>(Actor);
		if (SolidUnit)
		{
			StartBreak.ExecuteIfBound(SolidUnit);
		}
	}
}

void ALastController::Unselect(AActor* Actor)
{
	AEntity* Entity = Cast<AEntity>(Actor);
	if (Entity)
	{
		Entity->Unselect();
	}
	else
	{
		AUnit* Unit = Cast<AUnit>(Actor);
		if (Unit)
		{
			Unit->Unselect();

			if (bIsBreaking)
			{
				ASolidUnit* SolidUnit = Cast<ASolidUnit>(Unit);
				if (SolidUnit)
				{
					StopBreak.ExecuteIfBound();
				}
			}
		}
	}
	if (Selected == Actor)
	{
		Selected = nullptr;
	}
}

void ALastController::SetupInput()
{
	if (CurrentInputStack.Num() <= 0)
	{
		UE_LOG(LogGameplay, Fatal, TEXT("Last Controller: Input stack is empty"));
	}

	CurrentInputStack[0]->BindAxis("MoveX", this, &ALastController::MoveX_f);
	CurrentInputStack[0]->BindAxis("MoveY", this, &ALastController::MoveY_f);

	CurrentInputStack[0]->BindAction("ZoomIn", IE_Pressed, this, &ALastController::ZoomIn_f);
	CurrentInputStack[0]->BindAction("ZoomOut", IE_Pressed, this, &ALastController::ZoomOut_f);
	CurrentInputStack[0]->BindAction("Run", IE_Pressed, this, &ALastController::StartRun_f);
	CurrentInputStack[0]->BindAction("Run", IE_Released, this, &ALastController::StopRun_f);
	CurrentInputStack[0]->BindAction("Attack", IE_Pressed, this, &ALastController::StartAttack_f);
	CurrentInputStack[0]->BindAction("Attack", IE_Released, this, &ALastController::StopAttack_f);
	CurrentInputStack[0]->BindAction("Interact", IE_Pressed, this, &ALastController::SpawnCow_tmp);
	CurrentInputStack[0]->BindAction("SwitchLang", IE_Pressed, this, &ALastController::SwitchLang_tmp);
}

void ALastController::MoveX_f(float Value)
{
	MoveX.ExecuteIfBound(Value);
}

void ALastController::MoveY_f(float Value)
{
	MoveY.ExecuteIfBound(Value);
}

void ALastController::ZoomIn_f()
{
	ZoomIn.ExecuteIfBound();
}

void ALastController::ZoomOut_f()
{
	ZoomOut.ExecuteIfBound();
}

void ALastController::StartRun_f()
{
	StartRun.ExecuteIfBound();
}

void ALastController::StopRun_f()
{
	StopRun.ExecuteIfBound();
}

void ALastController::StartAttack_f()
{
	AEntity* Entity = Cast<AEntity>(Selected);
	if (Entity && Attack.IsBound())
	{
		Attack.Execute(Entity);
	}
	else
	{
		ASolidUnit* SolidUnit = Cast<ASolidUnit>(Selected);
		if (SolidUnit)
		{
			StartBreak.ExecuteIfBound(SolidUnit);
			bIsBreaking = true;
		}
	}
}

void ALastController::StopAttack_f()
{
	StopBreak.ExecuteIfBound();
	bIsBreaking = false;
}

void ALastController::SpawnCow_tmp()
{
	FVector2D Mouse;
	GetMousePosition(Mouse.X, Mouse.Y);
	GetWorld()->SpawnActor<AAnimal>(GAME_MODE->GetDatabase()->GetMobData(FGameplayTag::RequestGameplayTag(FName(TEXT("entity.animal.cow")))).Class,
		FVector(Mouse - GetWorld()->GetGameInstance()->GetEngine()->GetGameUserSettings()->GetScreenResolution() / 4, 0.f) + GetPawn()->GetActorLocation(),
		GetPawn()->GetActorRotation());
}

void ALastController::SwitchLang_tmp()
{
	ALangManager* LangManager = GAME_MODE->GetLangManager();
	FLangs Lang = LangManager->GetLang();
	if (Lang == FLangs::FR_FR)
	{
		Lang = FLangs::EN_GB;
	}
	else
	{
		Lang = static_cast<FLangs>(static_cast<uint8>(Lang) + 1);
	}
	LangManager->SetLang(Lang);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("%s %s"), *LangManager->GetString(FName("lang.changed")), *LangManager->GetString(FName("lang.name"))));
}
