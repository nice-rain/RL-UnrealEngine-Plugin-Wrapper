// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Copyright 2019 Rainy Labs, LLC.

#include "WrapperHelperTempate.h"
#include "WrapperHelperTempateStyle.h"
#include "WrapperHelperTempateCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"


//Include LoadObject
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

//Include UserWidget
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

static const FName WrapperHelperTempateTabName("WrapperHelperTempate");

#define LOCTEXT_NAMESPACE "FWrapperHelperTempateModule"

void FWrapperHelperTempateModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FWrapperHelperTempateStyle::Initialize();
	FWrapperHelperTempateStyle::ReloadTextures();

	FWrapperHelperTempateCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FWrapperHelperTempateCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FWrapperHelperTempateModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FWrapperHelperTempateModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FWrapperHelperTempateModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(WrapperHelperTempateTabName, FOnSpawnTab::CreateRaw(this, &FWrapperHelperTempateModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FWrapperHelperTempateTabTitle", "WrapperHelperTempate"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FWrapperHelperTempateModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FWrapperHelperTempateStyle::Shutdown();

	FWrapperHelperTempateCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(WrapperHelperTempateTabName);
}

TSharedRef<SDockTab> FWrapperHelperTempateModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	//Load our user widget from blueprint.
	LoadUserWidget();

	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FWrapperHelperTempateModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("WrapperHelperTempate.cpp"))
		);

	//Declare our DockTab and assign it to the template
	TSharedRef<SDockTab> NewDockTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
	
	//If we have loaded our widget, assign it
	if (CreatedWidget)
	{
		//Log our widget's name
		log(*CreatedWidget->GetName());

		//Store our widget
		TSharedRef<SWidget> UserSlateWidget = CreatedWidget->TakeWidget();

		//Assign our widget to our tab's content
		NewDockTab->SetContent(UserSlateWidget);

	}

	//Return our tab
	return NewDockTab;
}

void FWrapperHelperTempateModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(WrapperHelperTempateTabName);
}

void FWrapperHelperTempateModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FWrapperHelperTempateCommands::Get().OpenPluginWindow);
}

//This function does the magic of loading the entire widget from disk into an object.
//Once we've loaded it, we then create a widget and store it to replace our slate UI with.
void FWrapperHelperTempateModule::LoadUserWidget()
{

	logWarning(*FString("Function: LoadUserWidget()"));


	//This path is contained within the WrapperHelperTemplate Content folder
	UClass* LoadedWidget = LoadObject<UClass>(NULL, TEXT("/WrapperHelperTempate/BP_SlateWrapper.BP_SlateWrapper_C"), NULL, LOAD_None, NULL);
	if (LoadedWidget)
	{
		log(*FString("User Widget loaded successfully"));
		
		//If successfully loaded, store it for later use.
		CreatedWidget = CreateWidget(GEditor->GetEditorWorldContext().World(), LoadedWidget);
	}
	else
	{
		log(*FString("Unable to loacate BP_SlateWrapper inside our plugin's content folder."));
	}
}

void FWrapperHelperTempateModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FWrapperHelperTempateCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWrapperHelperTempateModule, WrapperHelperTempate)