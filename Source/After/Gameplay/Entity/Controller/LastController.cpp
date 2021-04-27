    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: LastController.cpp          //
////////////////////////////////////////

#include "LastController.h"

#include "../Entity.h"
//#include "../../Unit/Unit.h"

ALastController::ALastController() :
	APlayerController()
{
	SetShowMouseCursor(true);
	bEnableMouseOverEvents = true;
}

void ALastController::BeginPlay()
{
	Super::BeginPlay();
}

void ALastController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AEntity* SelectedEntity = Cast<AEntity>(Selected);
	if (SelectedEntity)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("*          Health: %f"), SelectedEntity->GetHealth()));
	}

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Selected: %s"), Selected ? *Selected->GetName() : *FString("None")));
}

void ALastController::Select(AActor* Actor)
{
	AEntity* Entity = Cast<AEntity>(Actor);
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
//		AUnit* Unit = Cast<AUnit>(Actor);
//		if (Unit)
//		{
//			Selected = Unit;
//			Unit->Select();
//			bNew = true;
//		}
	}

	if (bNew && Old)
	{
		Unselect(Old);
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
//		AUnit* Unit = Cast<AUnit>(Actor);
//		if (Unit)
//		{
//			Unit->Unselect();
//		}
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
		UE_LOG(LogTemp, Fatal, TEXT("Input stack is empty"));
	}

	CurrentInputStack[0]->BindAxis("MoveX", this, &ALastController::MoveX_f);
	CurrentInputStack[0]->BindAxis("MoveY", this, &ALastController::MoveY_f);

	CurrentInputStack[0]->BindAction("ZoomIn", IE_Pressed, this, &ALastController::ZoomIn_f);
	CurrentInputStack[0]->BindAction("ZoomOut", IE_Pressed, this, &ALastController::ZoomOut_f);
	CurrentInputStack[0]->BindAction("Run", IE_Pressed, this, &ALastController::StartRun_f);
	CurrentInputStack[0]->BindAction("Run", IE_Released, this, &ALastController::StopRun_f);
	CurrentInputStack[0]->BindAction("Attack", IE_Pressed, this, &ALastController::Attack_f);
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

void ALastController::Attack_f()
{
	AEntity* Entity = Cast<AEntity>(Selected);
	if (Entity && Attack.IsBound())
	{
		Attack.Execute(Entity);
	}
}
