// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Copyright 2019 Rainy Labs, LLC.


#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

//=======================================================
//Log Macro
//=======================================================

//log a string
#define log(text) UE_LOG(LogTemp, Log, TEXT("%s"), text);

//Logs warning
#define logWarning(text) UE_LOG(LogTemp, Warning, TEXT("%s"), text);

//Logs error
#define logError(text) UE_LOG(LogTemp, Error, TEXT("%s"), text);

class FWrapperHelperTempateModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	//Used to load our userWidget blueprint (which is a slate wrapper)
	void LoadUserWidget();

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	//Stored slate widget that was loaded
	class UUserWidget* CreatedWidget;

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
