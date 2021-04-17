    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Database.h                  //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "EntityInfo.h"
#include "UnitInfo.h"
#include "ItemInfo.h"

#include "Database.generated.h"

UCLASS()
class UDatabase : public UDataAsset
{
	GENERATED_BODY()

protected:
			/* ENTITY */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Entity")
	TMap<FGameplayTag, FEntityInfo> EntityData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Entity")
	TMap<FGameplayTag, FLastInfo> LastData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Entity")
	TMap<FGameplayTag, FMobInfo> MobData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Entity")
	TMap<FGameplayTag, FAnimalInfo> AnimalData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Entity")
	TMap<FGameplayTag, FWolfInfo> WolfData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Entity")
	TMap<FGameplayTag, FMutantInfo> MutantData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Entity")
	TMap<FGameplayTag, FAlienInfo> AlienData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Entity")
	TMap<FGameplayTag, FRobotInfo> RobotData;

			/* UNIT */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	TMap<FGameplayTag, FUnitInfo> UnitData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	TMap<FGameplayTag, FLiquidInfo> LiquidData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	TMap<FGameplayTag, FSolidUnitInfo> SolidUnitData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	TMap<FGameplayTag, FDesktopInfo> DesktopData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	TMap<FGameplayTag, FCrateInfo> CrateData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	TMap<FGameplayTag, FVesselUnitInfo> VesselUnitData;

			/* ITEM */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TMap<FGameplayTag, FItemInfo> ItemData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TMap<FGameplayTag, FVesselItemInfo> VesselItemData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TMap<FGameplayTag, FBuildableInfo> BuildableData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TMap<FGameplayTag, FFoodInfo> FoodData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TMap<FGameplayTag, FClothesInfo> ClothesData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TMap<FGameplayTag, FRangedWeaponInfo> RangedWeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TMap<FGameplayTag, FAmmunitionInfo> AmmunitionData;

public:
	UFUNCTION()
	void Check();
	
			/* ENTITY */

	UFUNCTION(BlueprintCallable, Category = "Entity")
	const FEntityInfo& GetEntityData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Entity")
	const FLastInfo& GetLastData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Entity")
	const FMobInfo& GetMobData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Entity")
	const FAnimalInfo& GetAnimalData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Entity")
	const FWolfInfo& GetWolfData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Entity")
	const FMutantInfo& GetMutantData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Entity")
	const FAlienInfo& GetAlienData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Entity")
	const FRobotInfo& GetRobotData(const FGameplayTag Tag) const;

			/* UNIT */

	UFUNCTION(BlueprintCallable, Category = "Unit")
	const FUnitInfo& GetUnitData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Unit")
	const FLiquidInfo& GetLiquidData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Unit")
	const FSolidUnitInfo& GetSolidUnitData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Unit")
	const FDesktopInfo& GetDesktopData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Unit")
	const FCrateInfo& GetCrateData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Unit")
	const FVesselUnitInfo& GetVesselUnitData(const FGameplayTag Tag) const;

			/* ITEM */

	UFUNCTION(BlueprintCallable, Category = "Item")
	const FItemInfo& GetItemData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Item")
	const FVesselItemInfo& GetVesselItemData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Item")
	const FBuildableInfo& GetBuildableData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Item")
	const FFoodInfo& GetFoodData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Item")
	const FClothesInfo& GetClothesData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Item")
	const FRangedWeaponInfo& GetRangedWeaponData(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Item")
	const FAmmunitionInfo& GetAmmunitionData(const FGameplayTag Tag) const;
};