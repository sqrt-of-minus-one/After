    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: Types.h                     //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

#include <functional>

#include "Types.generated.h"

class UPaperFlipbook;
class USoundBase;

//// 
// After any changement of any enum, definition of
// corresponding for_enum function also must be changed!
////

#define IS_TAG_PARENT(Tag, ParentString) (Tag.ToString().Find(ParentString) == 0)

// The function is used to make for loop for enums
// Func must make bool false if the loop must not continue
template <typename T>
inline void for_enum(const std::function<void(T, bool&)>& Func) = delete;

template<typename T>
static inline void _for_enum_impl(const std::function<void(T, bool&)>& Func, T Begin, T End)
{
	T i = Begin;
	bool Continue = true;
	do
	{
		Func(i, Continue);
		i = static_cast<T>(static_cast<int>(i) + 1);
	} while (Continue && i <= End);
}

UENUM(BlueprintType)
enum class FDamageType : uint8
{
	Strike,
	Cut,
	Pierce,
	Poison,
	Radiation,
	Suffocation,
	Internal,
	Burn
};
template<>
inline void for_enum(const std::function<void(FDamageType, bool&)>& Func)
{
	_for_enum_impl(Func, FDamageType::Strike, FDamageType::Burn);
}

UENUM(BlueprintType)
enum class FClothesType : uint8
{
	Hat,
	Jacket,
	Pants,
	Shoes,
	Backpack
};
template<>
inline void for_enum(const std::function<void(FClothesType, bool&)>& Func)
{
	_for_enum_impl(Func, FClothesType::Hat, FClothesType::Backpack);
}

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
template<>
inline void for_enum(const std::function<void(FEntityStatus, bool&)>& Func)
{
	_for_enum_impl(Func, FEntityStatus::Stay, FEntityStatus::Special);
}

UENUM(BlueprintType)
enum class FLiquidStatus : uint8
{
	Stay,
	SideFlow,
	DiagonalFlow
};
template<>
inline void for_enum(const std::function<void(FLiquidStatus, bool&)>& Func)
{
	_for_enum_impl(Func, FLiquidStatus::Stay, FLiquidStatus::DiagonalFlow);
}

UENUM(BlueprintType)
enum class FDirection : uint8
{
	F,
	R,
	B,
	L
};
template<>
inline void for_enum(const std::function<void(FDirection, bool&)>& Func)
{
	_for_enum_impl(Func, FDirection::F, FDirection::L);
}

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
template<>
inline void for_enum(const std::function<void(FEntitySoundType, bool&)>& Func)
{
	_for_enum_impl(Func, FEntitySoundType::Passive, FEntitySoundType::Death);
}

UENUM(BlueprintType)
enum class FLiquidSoundType : uint8
{
	Flow,
	EntityEnter,
	EntityQuit
};
template<>
inline void for_enum(const std::function<void(FLiquidSoundType, bool&)>& Func)
{
	_for_enum_impl(Func, FLiquidSoundType::Flow, FLiquidSoundType::EntityQuit);
}

UENUM(BlueprintType)
enum class FSolidUnitSoundType : uint8
{
	Passive,
	Steps,
	Placed,
	Breaking,
	Broken
};
template<>
inline void for_enum(const std::function<void(FSolidUnitSoundType, bool&)>& Func)
{
	_for_enum_impl(Func, FSolidUnitSoundType::Passive, FSolidUnitSoundType::Broken);
}

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

	// Item that is dropped <<item>>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag Item;

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