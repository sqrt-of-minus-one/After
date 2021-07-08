    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: AfterGameModeBase.cpp       //
////////////////////////////////////////

#include "AfterGameModeBase.h"

#include "Data/Database/Database.h"
#include "Data/Lang/LangManager.h"
#include "Gui/WidgetInitializer.h"
#include "Data/Database/LogDatabase.h"
#include "Data/Lang/LogLang.h"

AAfterGameModeBase::~AAfterGameModeBase()
{
	if (Database)
	{
		Database->Reset();
	}
}

void AAfterGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if (!Database)
	{
		UE_LOG(LogDatabase, Fatal, TEXT("Couldn't find database"));
	}
	Database->Init();

	LangManager = Cast<ALangManager>(GetWorld()->SpawnActor(LangManagerClass));
	UE_LOG(LogLang, Log, TEXT("Language manager object has been created (%s)"), *LangManager->GetName());

	WidgetInitializer = Cast<AWidgetInitializer>(GetWorld()->SpawnActor(WidgetInitializerClass));
}

const UDatabase* AAfterGameModeBase::GetDatabase() const
{
	return Database;
}

ALangManager* AAfterGameModeBase::GetLangManager()
{
	return LangManager;
}

AWidgetInitializer* AAfterGameModeBase::GetWidgetInitializer()
{
	return WidgetInitializer;
}
