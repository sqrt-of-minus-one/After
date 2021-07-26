    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: InventoryComponent.cpp      //
////////////////////////////////////////

#include "InventoryComponent.h"

#include "../../Gameplay/Item/Item.h"
#include "PlayerInventoryComponent.h"

UInventoryComponent::UInventoryComponent() :
	bInitialized(false),
	Fullness(0.f)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::Init(float Size)
{
	if (!bInitialized)
	{
		MaxFullness = Size;
		bInitialized = true;
	}
}

int UInventoryComponent::GetCurrentSize() const
{
	return Inventory.Num();
}

float UInventoryComponent::GetFullness() const
{
	return Fullness;
}

float UInventoryComponent::GetMaxFullness() const
{
	return MaxFullness;
}

AItem* UInventoryComponent::Get(int Index) const
{
	if (bInitialized && Index >= 0 && Index < Inventory.Num())
	{
		return Inventory[Index];
	}
	else
	{
		return nullptr;
	}
}

AItem* UInventoryComponent::Take(int Index, int Count)
{
	if (bInitialized && Index >= 0 && Index < Inventory.Num() && IsValid(Inventory[Index]) && Count > 0)
	{
		if (Count >= Inventory[Index]->GetCount()) // Remove the whole stack
		{
			AItem* Item = Inventory[Index];
			Inventory.RemoveAt(Index);
			Fullness -= Item->GetItemData().Weight * Item->GetCount();
			return Item;
		}
		else // Remove part of the stack
		{
			AItem* Item = GetWorld()->SpawnActor<AItem>(Inventory[Index]->GetClass());
			Item->SetCount(Count);
			// Item is not stackable (because otherwise "if" would have been true
			Inventory[Index]->SetCount(Inventory[Index]->GetCount() - Count);
			Fullness -= Item->GetItemData().Weight * Count;
			return Item;
		}
	}
	else
	{
		return nullptr;
	}
}

int UInventoryComponent::Put(AItem* Item, int Count)
{
	if (bInitialized)
	{
		// How many items can be put into inventory
		int MoveCount = FMath::Min3(Count, Item->GetCount(), static_cast<int>((MaxFullness - Fullness) / Item->GetItemData().Weight));
		if (MoveCount <= 0) // Missing space or attempt to put non-positive count of items
		{
			return 0;
		}
		else
		{
			if (Item->GetItemData().bIsStackable)
			{
				// Try to put items into existing stack
				for (int i = 0; i < Inventory.Num(); i++)
				{
					if (Inventory[i]->GetId() == Item->GetId())
					{
						Inventory[i]->SetCount(Inventory[i]->GetCount() + MoveCount);
						Fullness += Item->GetItemData().Weight * MoveCount;
						Item->SetCount(Item->GetCount() - MoveCount);
						return MoveCount;
					}
				}
			}
			// Create a new stack
			AItem* NewItem = GetWorld()->SpawnActor<AItem>(Item->GetClass());
			NewItem->SetCount(MoveCount);
			NewItem->OnItemBroken.AddDynamic(this, &UInventoryComponent::ItemBroken);
			Fullness += Item->GetItemData().Weight * MoveCount;
			Inventory.Add(NewItem);
			Item->SetCount(Item->GetCount() - MoveCount);
			return MoveCount;
		}
	}
	else
	{
		return -1;
	}
}

int UInventoryComponent::PutAll(AItem* Item)
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

int UInventoryComponent::MoveToInventory(int Index, int Count, UInventoryComponent* InventoryComponent)
{
	return MoveToInventory_(Index, Count, InventoryComponent);
}

int UInventoryComponent::MoveToPlayerInventory(int Index, int Count, UPlayerInventoryComponent* InventoryComponent)
{
	return MoveToInventory_(Index, Count, InventoryComponent);
}

void UInventoryComponent::ItemBroken(AItem* Item, float Weight)
{
	if (Item)
	{
		for (int i = 0; i < Inventory.Num(); i++)
		{
			if (Inventory[i] == Item)
			{
				Inventory.RemoveAt(i);
				Fullness -= Weight;
			}
		}
	}
}

template<typename T>
int UInventoryComponent::MoveToInventory_(int Index, int Count, T* InventoryComponent)
{
	if (bInitialized)
	{
		if (Index >= 0 && Index < Inventory.Num() && IsValid(Inventory[Index]) && Count > 0)
		{
			// Original count of items
			int FirstCount = Inventory[Index]->GetCount();
			float Weight = Inventory[Index]->GetItemData().Weight;

			// Move some items to other inventory
			int Moved = InventoryComponent->Put(Inventory[Index], Count);
			if (Moved > 0)
			{
				if (Moved >= FirstCount) // If all of the items were moved
				{
					Inventory.RemoveAt(Index);
					Fullness -= FirstCount * Weight;
				}
				else
				{
					Fullness -= Moved * Weight;
				}
			}
			return Moved;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}
