    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: InventoryComponent.cpp      //
////////////////////////////////////////

#include "InventoryComponent.h"

#include "../Gameplay/Item/Item.h"

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
			Inventory[Index] = nullptr;
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

int UInventoryComponent::Put(AItem* Item)
{
	if (bInitialized)
	{
		int Count = FMath::Min(Item->GetCount(), static_cast<int>((MaxFullness - Fullness) / Item->GetItemData().Weight));
		if (Count <= 0) // Missing space
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
						Inventory[i]->SetCount(Inventory[i]->GetCount() + Count);
						Fullness += Item->GetItemData().Weight * Count;
						Item->SetCount(Item->GetCount() - Count);
						return Count;
					}
				}
			}
			AItem* NewItem = GetWorld()->SpawnActor<AItem>(Item->GetClass());
			NewItem->SetCount(Count);
			Fullness += Item->GetItemData().Weight * Count;
			Inventory.Add(NewItem);
			Item->SetCount(Item->GetCount() - Count);
			return Count;
		}
	}
	else
	{
		return -1;
	}
}
