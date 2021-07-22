    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: ThrownItem.h                //
////////////////////////////////////////

#include "ThrownItem.h"

#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"

#include "Item.h"
#include "../../GameConstants.h"
#include "../../Data/Database/Database.h"
#include "../../AfterGameModeBase.h"
#include "../Entity/Controller/LastController.h"
#include "../LogGameplay.h"

AThrownItem::AThrownItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetBoxExtent(GameConstants::ThrownItemSize);
	CollisionComponent->SetCollisionProfileName(TEXT("ThrownItem"));

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	SpriteComponent->SetupAttachment(GetRootComponent());
	SpriteComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));

	SelectionSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Selection Sprite"));
	SelectionSpriteComponent->SetupAttachment(SpriteComponent);
	SelectionSpriteComponent->SetVisibility(false);
}

void AThrownItem::BeginPlay()
{
	Super::BeginPlay();
}

void AThrownItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AItem* AThrownItem::GetItem() const
{
	return Item;
}

bool AThrownItem::SetItem(AItem* ItemToSet)
{
	if (!Item && IsValid(ItemToSet))
	{
		Item = ItemToSet;
		
		const FItemInfo& ItemData = Item->GetItemData();
		if (ItemData.bUseFlipbook)
		{
			FlipbookComponent->SetFlipbook(ItemData.WorldFlipbook);
			FlipbookComponent->SetRelativeScale3D(GameConstants::ThrownItemSize / GameConstants::TileSize);
			SelectionSpriteComponent->AttachToComponent(FlipbookComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
			MeshComponent = FlipbookComponent;
			SpriteComponent->DestroyComponent();
			SpriteComponent = nullptr;
		}
		else
		{
			SpriteComponent->SetSprite(ItemData.WorldSprite);
			SpriteComponent->SetRelativeScale3D(GameConstants::ThrownItemSize / GameConstants::TileSize);
			MeshComponent = SpriteComponent;
			FlipbookComponent->DestroyComponent();
			FlipbookComponent = nullptr;
		}

		ALastController* LastController = Cast<ALastController>(GetWorld()->GetFirstPlayerController());
		if (LastController)
		{
			OnBeginCursorOver.AddDynamic(LastController, &ALastController::Select);
			OnBeginCursorOver.AddDynamic(this, &AThrownItem::Select);
			OnEndCursorOver.AddDynamic(LastController, &ALastController::Unselect);
			OnEndCursorOver.AddDynamic(this, &AThrownItem::Unselect);
		}
		else
		{
			UE_LOG(LogGameplay, Error, TEXT("Couldn't find Last Controller"));
		}

		const UDatabase* Database = GAME_MODE->GetDatabase();
		if (!Database->GetExtraData().SelectionSprites.Contains(FIntPoint(1, 1)) ||
			!Database->GetExtraData().SelectionSprites[FIntPoint(1, 1)])
		{
			UE_LOG(LogGameplay, Error, TEXT("Database doesn't contain selection sprite with size 1x1"));
		}
		else
		{
			SelectionSpriteComponent->SetSprite(Database->GetExtraData().SelectionSprites[FIntPoint(1, 1)]);
		}

		return true;
	}
	else
	{
		return false;
	}
}

void AThrownItem::Select(AActor* Actor)
{
	SelectionSpriteComponent->SetVisibility(true);
}

void AThrownItem::Unselect(AActor* Actor)
{
	SelectionSpriteComponent->SetVisibility(false);
}
