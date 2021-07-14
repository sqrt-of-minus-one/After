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

	// References for lambda
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

void UPlayerInventoryComponent::Init(float Size, int HotbarSize)
{
	Inventory->Init(Size);
	Hotbar.SetNum(Size);
	for (int& i : Hotbar)
	{
		i = -1;
	}
	bInitialized = true;
}

float UPlayerInventoryComponent::GetFullness() const
{
	float Res = Inventory->GetFullness();

	// References for lambda
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

	// References for lambda
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

int UPlayerInventoryComponent::Put(AItem* Item, int Count)
{
	// Original count of items
	int FirstCount = Count;

	// How many items are left
	int MoveCount = FirstCount;

	// Try to put items into main inventory
	MoveCount -= Inventory->Put(Item, MoveCount);

	// References for lambda
	TMap<FClothesType, AItem*>& ClothesRef = Clothes;
	TMap<FClothesType, UInventoryComponent*>& ClothesInventoryRef = ClothesInventory;
	// Try to put items into clothes
	for_enum<FClothesType>([&MoveCount, &ClothesRef, &ClothesInventoryRef, Item](FClothesType i, bool& out_continue)
	{
		if (MoveCount <= 0) // If all of the items were put
		{
			out_continue = false;
		}
		else if (IsValid(ClothesRef[i]) && IsValid(ClothesInventoryRef[i]))
		{
			MoveCount -= ClothesInventoryRef[i]->Put(Item, MoveCount);
		}
	});
	return FirstCount - MoveCount;
}

int UPlayerInventoryComponent::PutAll(AItem* Item)
{
	if (IsValid(Item))
	{
		return Put(Item, Item->GetCount());
	}
	else
	{
		return Put(Item, 0);
	}
}

AItem* UPlayerInventoryComponent::GetHotbarItem(int Index) const
{
	if (Index >= 0 && Index < Hotbar.Num())
	{
		return Inventory->Get(Hotbar[Index]);
	}
	else
	{
		return nullptr;
	}
}

void UPlayerInventoryComponent::SetHotbarItem(int HotbarIndex, int ItemIndex)
{
	Hotbar[HotbarIndex] = ItemIndex;
}
