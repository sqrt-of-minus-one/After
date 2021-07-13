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
			// Item should not be stackable (because otherwise "if" would have been true
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
		int MoveCount = FMath::Min3(Count, Item->GetCount(), static_cast<int>((MaxFullness - Fullness) / Item->GetItemData().Weight));
		if (MoveCount <= 0) // Missing space
		{
			return 0;
		}
		else
		{
			if (Item->GetItemData().bIsStackable)
			{
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
			AItem* NewItem = GetWorld()->SpawnActor<AItem>(Item->GetClass());
			NewItem->SetCount(MoveCount);
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
	if (bInitialized)
	{
		if (Index >= 0 && Index < Inventory.Num() && IsValid(Inventory[Index]) && Count > 0)
		{
			int FirstCount = Inventory[Index]->GetCount();
			float Weight = Inventory[Index]->GetItemData().Weight;
			int Moved = InventoryComponent->Put(Inventory[Index], Count);
			if (Moved >= FirstCount)
			{
				Inventory.RemoveAt(Index);
				Fullness -= FirstCount * Weight;
			}
			else
			{
				Fullness -= Moved * Weight;
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

int UInventoryComponent::MoveToPlayerInventory(int Index, int Count, UPlayerInventoryComponent* InventoryComponent)
{
	if (bInitialized)
	{
		if (Index >= 0 && Index < Inventory.Num() && IsValid(Inventory[Index]) && Count > 0)
		{
			int FirstCount = Inventory[Index]->GetCount();
			float Weight = Inventory[Index]->GetItemData().Weight;
			int Moved = InventoryComponent->Put(Inventory[Index], Count);
			if (Moved >= FirstCount)
			{
				Inventory.RemoveAt(Index);
				Fullness -= FirstCount * Weight;
			}
			else
			{
				Fullness -= Moved * Weight;
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
