    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: WidgetInitializer.h         //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WidgetInitializer.generated.h"

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

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetCrateInventoryWidget() const;

	UFUNCTION(BlueprintImplementableEvent)
	void CrateInventoryInit(UUserWidget* Widget, ACrate* Crate);

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> CrateInventoryWidget;

};
