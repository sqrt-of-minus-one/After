    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: LastController.cpp          //
////////////////////////////////////////

#include "LastController.h"

#include "GameFramework/GameUserSettings.h"
#include "Blueprint/UserWidget.h"

#include "../../../Data/Database/Database.h"
#include "../../../Data/Lang/LangManager.h"
#include "../../LogGameplay.h"
#include "../Last.h"
#include "../../Unit/SolidUnit/SolidUnit.h"
#include "../../Item/ThrownItem.h"
#include "../../Item/Item.h"
#include "../../../AfterGameModeBase.h"
#include "GameplayTagContainer.h"
#include "../Mob/Animal.h"
#include "../../../Gui/WidgetInitializer.h"
#include "../../../GameConstants.h"

ALastController::ALastController() :
	bIsBreaking(false),
	bIsDead(false),
	Item(nullptr)
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

	if (Item && Item->IsPendingKill())
	{
		Item = nullptr;
		SetItem.ExecuteIfBound(Item);
	}


	const ALangManager* LangManager = GAME_MODE->GetLangManager();

	AEntity* SelectedEntity = nullptr;
	AUnit* SelectedUnit = nullptr;
	AThrownItem* SelectedThrownItem = nullptr;
	if (SelectedEntity = Cast<AEntity>(Selected), IsValid(SelectedEntity))
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Orange, FString::Printf(TEXT("*          %s: %f"), *LangManager->GetString(FName("stats.health")), SelectedEntity->GetHealth()));

		FString EntityName = GAME_MODE->GetLangManager()->GetString(FName(SelectedEntity->GetId().ToString() + FString(".name")));

		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("%s: %s"), *LangManager->GetString(FName("tmp.selected")), *EntityName));
	}
	else if (SelectedUnit = Cast<AUnit>(Selected), IsValid(SelectedUnit))
	{
		FString UnitName = GAME_MODE->GetLangManager()->GetString(FName(SelectedUnit->GetId().ToString() + FString(".name")));

		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("%s: %s"), *LangManager->GetString(FName("tmp.selected")), *UnitName));
	}
	else if (SelectedThrownItem = Cast<AThrownItem>(Selected), IsValid(SelectedThrownItem))
	{
		FString ThrownItemName = GAME_MODE->GetLangManager()->GetString(FName(SelectedThrownItem->GetItem()->GetId().ToString() + FString(".name")));

		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("%s: %s"), *LangManager->GetString(FName("tmp.selected")), *ThrownItemName));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("%s: None"), *LangManager->GetString(FName("tmp.selected"))));
	}
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("\n")));
	if (IsValid(Item))
	{
		if (Item->GetItemData().MaxCondition > 0.f)
		{
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, FString::Printf(TEXT("%s (%f / %f)"), *GAME_MODE->GetLangManager()->GetString(FName(Item->GetId().ToString() + FString(".name"))), Item->GetCondition(), Item->GetItemData().MaxCondition));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, *GAME_MODE->GetLangManager()->GetString(FName(Item->GetId().ToString() + FString(".name"))));
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("\n")));
}

void ALastController::Select(AActor* Actor)
{
	AEntity* Entity = Cast<AEntity>(Actor);
	bool bNew = false;
	AActor* Old = Selected;
	if (IsValid(Entity))
	{
		Selected = Entity;
		Entity->Select();
		bNew = true;
	}
	else
	{
		AUnit* Unit = Cast<AUnit>(Actor);
		if (IsValid(Unit))
		{
			Selected = Unit;
			Unit->Select();
			bNew = true;
		}
		else
		{
			AThrownItem* ThrownItem = Cast<AThrownItem>(Actor);
			if (IsValid(ThrownItem))
			{
				Selected = ThrownItem;
				ThrownItem->Select();
				bNew = true;
			}
		}
	}

	if (bNew && Old)
	{
		Unselect(Old);
	}

	if (bIsBreaking)
	{
		ASolidUnit* SolidUnit = Cast<ASolidUnit>(Actor);
		if (IsValid(SolidUnit))
		{
			StartBreak.ExecuteIfBound(SolidUnit, Item);
		}
	}
}

void ALastController::Unselect(AActor* Actor)
{
	AEntity* Entity = Cast<AEntity>(Actor);
	if (IsValid(Entity))
	{
		Entity->Unselect();
	}
	else
	{
		AUnit* Unit = Cast<AUnit>(Actor);
		if (IsValid(Unit))
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
		else
		{
			AThrownItem* ThrownItem = Cast<AThrownItem>(Actor);
			if (IsValid(ThrownItem))
			{
				ThrownItem->Unselect();
			}
		}
	}
	if (Selected == Actor)
	{
		Selected = nullptr;
	}
}

void ALastController::Death()
{
	// Unbinds all the delegate but zoom.
	// UnBind function doesn't work and I don't know why. That's why empty lambdas are used.
	StopAttack_f();
	MoveX.BindLambda([](float) {});
	MoveY.BindLambda([](float) {});
	StartRun.BindLambda([]() {});
	StopRun.BindLambda([]() {});
	Attack.BindLambda([](AEntity*, bool, AItem*) { return false; });
	StartBreak.BindLambda([](ASolidUnit*, AItem*) {});
	StopBreak.BindLambda([]() {});
	SetItem.BindLambda([](AItem*) {});
	bIsDead = true;
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
	CurrentInputStack[0]->BindAction("Interact", IE_Pressed, this, &ALastController::Interact_f);
	CurrentInputStack[0]->BindAction("SwitchLang", IE_Pressed, this, &ALastController::SwitchLang_tmp);
	CurrentInputStack[0]->BindAction("Throw", IE_Pressed, this, &ALastController::Throw_f);
	CurrentInputStack[0]->BindAction("PlayerMenu", IE_Pressed, this, &ALastController::Menu_f);
	CurrentInputStack[0]->BindAction("Inventory", IE_Pressed, this, &ALastController::Inventory_f);
	CurrentInputStack[0]->BindAction("Crafting", IE_Pressed, this, &ALastController::Crafting_f);
	CurrentInputStack[0]->BindAction("Skills", IE_Pressed, this, &ALastController::Skills_f);
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
	if (!bIsDead)
	{
		AEntity* Entity = Cast<AEntity>(Selected);
		if (IsValid(Entity) && Attack.IsBound())
		{
			Attack.Execute(Entity, true, Item);
		}
		else
		{
			AThrownItem* ThrownItem = Cast<AThrownItem>(Selected);
			if (IsValid(ThrownItem) && !Item &&
				FVector::DistSquared(ThrownItem->GetActorLocation(), GetPawn()->GetActorLocation()) <= FMath::Square(Cast<AEntity>(GetPawn())->GetEntityData().AttackRadius))
			{
				Item = ThrownItem->GetItem();
				GetWorld()->DestroyActor(ThrownItem);
			}
			else
			{
				ASolidUnit* SolidUnit = Cast<ASolidUnit>(Selected);
				if (IsValid(SolidUnit))
				{
					StartBreak.ExecuteIfBound(SolidUnit, Item);
					bIsBreaking = true;
				}
			}
		}
	}
}

void ALastController::StopAttack_f()
{
	StopBreak.ExecuteIfBound();
	bIsBreaking = false;
}

void ALastController::Interact_f()
{
	ASolidUnit* SolidUnit = Cast<ASolidUnit>(Selected);
	if (IsValid(SolidUnit) &&
		FVector::DistSquared(SolidUnit->GetActorLocation(), GetPawn()->GetActorLocation()) <= FMath::Square(Cast<AEntity>(GetPawn())->GetEntityData().AttackRadius))
	{
		SolidUnit->Interact(Cast<ALast>(GetPawn()));
	}
	else
	{
		GetWorld()->SpawnActor<AAnimal>(GAME_MODE->GetDatabase()->GetMobData(FGameplayTag::RequestGameplayTag(FName(TEXT("entity.animal.cow")))).Class,
			FVector(0.f, 0.f, GetPawn()->GetActorLocation().Z), GetPawn()->GetActorRotation());
	}
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

void ALastController::Throw_f()
{
	if (!bIsDead && IsValid(Item))
	{
		AThrownItem* Drop = GetWorld()->SpawnActor<AThrownItem>(GAME_MODE->GetDatabase()->GetExtraData().ThrownItemClass.Get(), GetPawn()->GetActorLocation() + FVector(Cast<AEntity>(GetPawn())->GetEntityData().Size, 0.f) * GameConstants::TileSize * FMath::RandRange(-.5f, .5f), FRotator(0.f, 0.f, 0.f));
		Drop->SetItem(Item);
		Item = nullptr;
		SetItem.ExecuteIfBound(Item);
	}
}

void ALastController::Menu_f()
{
	OpenMenu(FMenuType::Menu);
}

void ALastController::Inventory_f()
{
	OpenMenu(FMenuType::Inventory);
}

void ALastController::Crafting_f()
{
	OpenMenu(FMenuType::Crafting);
}

void ALastController::Skills_f()
{
	OpenMenu(FMenuType::Skills);
}

void ALastController::OpenMenu(FMenuType Type)
{
	AAfterGameModeBase* GameMode = GAME_MODE;
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), GameMode->GetWidgetInitializer()->GetPlayerMenuWidget());
	GameMode->GetWidgetInitializer()->PlayerMenuInit(Widget, Cast<ALast>(GetPawn()), Type);
	if (Widget)
	{
		Widget->AddToViewport(1);
	}
}
