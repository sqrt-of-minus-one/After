    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: ThrownItem.h                //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ThrownItem.generated.h"

#define RAND_ITEM_POSITION(Position)  \
	Position + GameConstants::TileSize * FMath::RandRange(-.5f, .5f)

#define THROW_ITEM(Item, Position) \
	(GetWorld()->SpawnActor<AThrownItem>(GAME_MODE->GetDatabase()->GetExtraData().ThrownItemClass.Get(), Position, FRotator(0.f, 0.f, 0.f)))->SetItem(Item)

#define SPAWN_THROWN_ITEM(Id, Count, Position) \
	{ \
		AItem* Item = GetWorld()->SpawnActor<AItem>(GAME_MODE->GetDatabase()->GetItemData(Id).Class.Get()); \
		Item->SetCount(Count); \
		THROW_ITEM(Item, Position); \
	}

class AItem;
class UBoxComponent;
class UPaperFlipbookComponent;
class UPaperSpriteComponent;
class UMeshComponent;

UCLASS()
class AFTER_API AThrownItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AThrownItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

// ==================================================

public:
			/* GENERAL */

	UFUNCTION(BlueprintCallable, Category = "General")
	AItem* GetItem() const;

	UFUNCTION(BlueprintCallable, Category = "General")
	bool SetItem(AItem* ItemToSet); // Might be called only once

			/* SELECTION */

	UFUNCTION(Category = "Selection")
	void Select(AActor* Actor);

	UFUNCTION(Category = "Selection")
	void Unselect(AActor* Actor);

protected:
			/* GENERAL */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "General")
	AItem* Item;

			/* COMPONENTS */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperFlipbookComponent* FlipbookComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* SelectionSpriteComponent;

};
