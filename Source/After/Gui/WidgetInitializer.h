    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: WidgetInitializer.h         //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WidgetInitializer.generated.h"

enum class FMenuType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWidgetCreatedEvent);

UCLASS()
class AFTER_API AWidgetInitializer : public AActor
{
	GENERATED_BODY()
	
public:	
	AWidgetInitializer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ==================================================

public:

	UPROPERTY(BlueprintAssignable)
	FOnWidgetCreatedEvent OnWidgetCreated;

	UFUNCTION(BlueprintCallable)
	void CloseWidget();

	UFUNCTION(BlueprintCallable)
	void DisplayMainWidget(ALast* Last);

	UFUNCTION(BlueprintCallable)
	void DisplayCrateInventoryWidget(ACrate* Crate, ALast* Last);

	UFUNCTION(BlueprintCallable)
	void DisplayPlayerMenuWidget(ALast* Last, FMenuType Type);

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UUserWidget* CurrentWidget;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool bIsMainWidgetCreated;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> MainWidget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> CrateInventoryWidget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> PlayerMenuWidget;

	UFUNCTION(BlueprintImplementableEvent)
	void MainInit(UUserWidget* Widget, ALast* Last);

	UFUNCTION(BlueprintImplementableEvent)
	void CrateInventoryInit(UUserWidget* Widget, ACrate* Crate, ALast* Last);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerMenuInit(UUserWidget* Widget, ALast* Last, FMenuType Type);

};
