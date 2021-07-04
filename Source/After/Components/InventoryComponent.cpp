    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: InventoryComponent.cpp      //
////////////////////////////////////////

#include "InventoryComponent.h"

#include "../Gameplay/Item/Item.h"

UInventoryComponent::UInventoryComponent() :
	bInitialized(false)
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

void UInventoryComponent::Init(int Size)
{
	if (!bInitialized)
	{
		Inventory.SetNum(Size);
		bInitialized = true;
	}
}

const AItem* UInventoryComponent::Get(int Index) const
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

AItem* UInventoryComponent::Remove(int Index, int Count)
{
	if (bInitialized && Index >= 0 && Index < Inventory.Num() && IsValid(Inventory[Index]) && Count > 0)
	{
		if (Count >= Inventory[Index]->GetCount()) // Remove the whole stack
		{
			AItem* Item = Inventory[Index];
			Inventory[Index] = nullptr;
			return Item;
		}
		else // Remove part of the stack
		{
			AItem* Item = GetWorld()->SpawnActor<AItem>(Inventory[Index]->GetClass());
			int ActualCount = Item->SetCount(Count);
			Inventory[Index]->SetCount(Inventory[Index]->GetCount() - ActualCount);
			return Item;
		}
	}
	else
	{
		return nullptr;
	}
}

int UInventoryComponent::PutTo(AItem* Item, int Index)
{
	if (bInitialized && Index >= 0 && Index < Inventory.Num())
	{
		if (IsValid(Inventory[Index])) // Inventory cell is filled
		{
			if (Inventory[Index]->GetId() == Item->GetId()) // Item being added and item in the cell are the same
			{
				int Count = Inventory[Index]->SetCount(Inventory[Index]->GetCount() + Item->GetCount());
				Item->SetCount(Item->GetCount() - Count);
				return Count;
			}
			else
			{
				return 0;
			}
		}
		else // Inventory cell is empty
		{
			Inventory[Index] = Item;
			return Item->GetCount();
		}
	}
	else
	{
		return 0;
	}
}

int UInventoryComponent::Put(AItem* Item)
{
	if (bInitialized)
	{
		int Ret = 0;
		for (int i = 0; i < Inventory.Num(); i++) // Try to add items in filled cells
		{
			if (IsValid(Inventory[i])) // If the cell is filled
			{
				int FirstCount = Item->GetCount();
				int Count = PutTo(Item, i); // Try to put the items
				Ret += Count;
				if (Count >= FirstCount) // If all of the items were put
				{
					return Ret;
				}
			}
		}
		for (AItem* i : Inventory) // Try to add items in empty cells
		{
			if (!IsValid(i)) // If the cell is empty
			{
				i = Item;
				return Ret + Item->GetCount();
			}
		}
		return Ret;
	}
	else
	{
		return 0;
	}
}
