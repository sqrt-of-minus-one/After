    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Types.h                     //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

#include "Types.generated.h"

class UPaperFlipbook;
class USoundBase;
class UItem;

UENUM(BlueprintType)
enum class FDamageType : uint8
{
	Strike,
	Cut,
	Pierce,
	Poison,
	Radiation,
	Suffocation,
	Burn
};

UENUM(BlueprintType)
enum class FClothesType : uint8
{
	Hat,
	Jacket,
	Pants,
	Shoes,
	Backpack
};

// What is entity doing now?
UENUM(BlueprintType)
enum class FEntityStatus : uint8
{
	Stay,
	Walk,
	Run,
	SwimStay,
	SwimMove,
	MeleeAttack,
	RangedAttack,
	Death,
	Damage,
	Stone,
	Web,
	Special
};

UENUM(BlueprintType)
enum class FDirection : uint8
{
	F,
	R,
	B,
	L
};

USTRUCT(BlueprintType)
struct FEntityFlipbooks
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FDirection, UPaperFlipbook*> Flipbooks;
};

UENUM(BlueprintType)
enum class FEntitySoundType : uint8
{
	Passive,
	Angry,
	Damage,
	Attack,
	Death
};

UENUM(BlueprintType)
enum class FLiquidSoundType : uint8
{
	Flow,
	EntityEnter,
	EntityQuit
};

UENUM(BlueprintType)
enum class FSolidUnitSoundType : uint8
{
	Passive,
	Steps,
	Placed,
	Breaking,
	Broken
};

USTRUCT(BlueprintType)
struct FSoundsArray
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<USoundBase*> Sounds;
};

USTRUCT(BlueprintType)
struct FEntitySounds
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FEntitySoundType, FSoundsArray> Sounds;
};

USTRUCT(BlueprintType)
struct FLiquidSounds
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FLiquidSoundType, FSoundsArray> Sounds;
};

USTRUCT(BlueprintType)
struct FSolidUnitSounds
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FSolidUnitSoundType, FSoundsArray> Sounds;
};

USTRUCT(BlueprintType)
struct FItemDrop
{
	GENERATED_USTRUCT_BODY()

public:

	// Item that is dropped
/* TODO
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UItem* Item; */

	// Minimum number of items
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Min;

	// Maximum number of items
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Max;

	// Probability that the item will be dropped
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Chance;
};
