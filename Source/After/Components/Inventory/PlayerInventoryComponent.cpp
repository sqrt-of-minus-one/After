    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: PlayerInventoryComponent.cpp//
////////////////////////////////////////

#include "PlayerInventoryComponent.h"

#include "InventoryComponent.h"
#include "../../Gameplay/Item/Item.h"

UPlayerInventoryComponent::UPlayerInventoryComponent() :
	bInitialized(false)
{
	PrimaryComponentTick.bCanEverTick = false;

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	TMap<FClothesType, AItem*>& ClothesRef = Clothes;
	TMap<FClothesType, UInventoryComponent*>& ClothesInventoryRef = ClothesInventory;

	for_enum<FClothesType>([&ClothesRef, &ClothesInventoryRef](FClothesType i, bool& out_continue)
	{
		ClothesRef.Add(i, nullptr);
		ClothesInventoryRef.Add(i, nullptr);
	});
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerInventoryComponent::Init(float Size)
{
	Inventory->Init(Size);
	bInitialized = true;
}

float UPlayerInventoryComponent::GetFullness() const
{
	float Res = Inventory->GetFullness();

	const TMap<FClothesType, AItem*>& ClothesRef = Clothes;
	const TMap<FClothesType, UInventoryComponent*>& ClothesInventoryRef = ClothesInventory;
	for_enum<FClothesType>([&Res, &ClothesRef, &ClothesInventoryRef](FClothesType i, bool& out_continue)
	{
		if (IsValid(ClothesRef[i]) && IsValid(ClothesInventoryRef[i]))
		{
			Res += ClothesInventoryRef[i]->GetFullness();
		}
	});

	return Res;
}

float UPlayerInventoryComponent::GetMaxFullness() const
{
	float Res = Inventory->GetMaxFullness();;

	const TMap<FClothesType, AItem*>& ClothesRef = Clothes;
	const TMap<FClothesType, UInventoryComponent*>& ClothesInventoryRef = ClothesInventory;
	for_enum<FClothesType>([&Res, &ClothesRef, &ClothesInventoryRef](FClothesType i, bool& out_continue)
	{
		if (IsValid(ClothesRef[i]) && IsValid(ClothesInventoryRef[i]))
		{
			Res += ClothesInventoryRef[i]->GetMaxFullness();
		}
	});

	return Res;
}

UInventoryComponent* UPlayerInventoryComponent::GetInventory() const
{
	return Inventory;
}

AItem* UPlayerInventoryComponent::GetClothes(FClothesType Type) const
{
	return Clothes[Type];
}

UInventoryComponent* UPlayerInventoryComponent::GetClothesInventory(FClothesType Type) const
{
	return ClothesInventory[Type];
}

int UPlayerInventoryComponent::Put(AItem* Item)
{
	int FirstCount = Item->GetCount();
	int Count = FirstCount;
	Count -= Inventory->Put(Item);

	TMap<FClothesType, AItem*>& ClothesRef = Clothes;
	TMap<FClothesType, UInventoryComponent*>& ClothesInventoryRef = ClothesInventory;
	for_enum<FClothesType>([&Count, &ClothesRef, &ClothesInventoryRef, Item](FClothesType i, bool& out_continue)
	{
		if (Count <= 0)
		{
			out_continue = false;
		}
		else if (IsValid(ClothesRef[i]) && IsValid(ClothesInventoryRef[i]))
		{
			Count -= ClothesInventoryRef[i]->Put(Item);
		}
	});
	return FirstCount - Count;
}
