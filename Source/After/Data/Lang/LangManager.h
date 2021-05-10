    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: LangManager.h               //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "LangManager.generated.h"

UENUM(BlueprintType)
enum class FLangs : uint8
{
	EN_GB,	// English (Great Britain)
	RU_RU,	// Russian (Russia)
	FR_FR,	// French (France)
	DL_DL	// Dalnian (Dalnia)
};

UCLASS(Abstract)
class ALangManager : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Language")
	FString GetString(const FName& Key) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Language")
	FLangs GetLang() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Language")
	bool SetLang(FLangs LangCode);
};
