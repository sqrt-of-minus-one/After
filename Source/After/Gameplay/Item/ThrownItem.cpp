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

AThrownItem::AThrownItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetBoxExtent(GameConstants::ThrownItemSize);
	CollisionComponent->SetCollisionProfileName(TEXT("ThrownItem"));

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	FlipbookComponent->SetupAttachment(GetRootComponent());

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	SpriteComponent->SetupAttachment(GetRootComponent());
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
	if (Item)
	{
		return false;
	}
	else
	{
		Item = ItemToSet;
		
		const FItemInfo& ItemData = Item->GetItemData();
		if (ItemData.bUseFlipbook)
		{
			FlipbookComponent->SetFlipbook(ItemData.Flipbook);
			MeshComponent = FlipbookComponent;
			SpriteComponent->DestroyComponent();
			SpriteComponent = nullptr;
		}
		else
		{
			SpriteComponent->SetSprite(ItemData.Sprite);
			MeshComponent = SpriteComponent;
			FlipbookComponent->DestroyComponent();
			FlipbookComponent = nullptr;
		}

		return true;
	}
}
