# Unreal Engine UW Plugin Wrapper

In order to develop plugin (modules) for UE4's editor UI, you would need to use Slate to design and style the layout. Because Slate can be quite complex, Epic Games created User Widgets to wrap Slate into a blueprint for designing in-game UI elements. Functions may also be written in blueprints as well in a User Widget Blueprint. This template is a wrapper that loads a User Widget and then sets the slate component's content to the slate component wrapped in the user widget. It also includes a static blueprint library to allow editor function access within a blueprint widget as well (easiest way I found of handling this).

TL;DR: This is a template/example that allows you to design editor UI elements using UMG. It allows you to write editor functions in C++ and Blueprints.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

* Unreal Engine 4.21 (The example has been tested on older versions)
* Visual Studio Community 2017
* This wrapper template

### Installing the Example

1. Clone this repository

```
git clone https://github.com/nice-rain/RL-UnrealEngine-Plugin-Wrapper.git
```

2. Run the example project (the plugin should auto-load and been seen within the editor)

![Plugin Loaded Successfully](https://NiceRa.in/rain/sharex/screenshots/UE4Editor_2019-01-06_20-37-35.png)

3. Clicking the C++ Function button will call a function from the plugin's Blueprint Static Library.

4. Clicking the BP Function button will call a function contained within our user widget that we loaded.

## Using this in your own plugin

Because this is a template, it probably isn't best to develop a distributed plugin with it (you'd run into conflicts in naming). Below are the steps I took to wrap a UW in a new project.

1. Create the new C++ project in Unreal Engine 4.

![New Project](https://NiceRa.in/rain/sharex/screenshots/UE4Editor_2019-01-06_20-35-52.png)

2. Open the Project and navigate to Edit-> Plugins

![Plugins](https://NiceRa.in/rain/sharex/screenshots/UE4Editor_2019-01-06_20-40-27.png)

3. Click New Plugin and select Editor Standalone Window (this will give us the plugin base). Enter a plugin name and click Create Plugin.
![New Plugin](https://NiceRa.in/rain/sharex/screenshots/UE4Editor_2019-01-06_20-42-28.png)

:exclamation: Because this plugin hooks at editor start, once the engine compiles. Go ahead and restart the editor to see the plugin button at the top.

4. Go back to Edit->Plugins and scroll all the way to the bottom of the list where it says 'Project'. Click 'Other' and you should be able to click your plugin from the window in front of you.

![New Plugin Success](https://NiceRa.in/rain/sharex/screenshots/UE4Editor_2019-01-06_20-48-46.png)

5. Click 'edit', fill out relevant information, and check the box :ballot_box_with_check: Can Contain Content. This allows us to cook our plugin if we need to (most editor extensions don't need to be cooked).

![Plugin Options](https://NiceRa.in/rain/sharex/screenshots/UE4Editor_2019-01-06_20-51-36.png)

:exclamation: Because the content folder is created when the editor loads, go ahead and restart the editor to generate the folder.

6. Within your project's Content Browser, you need to click the show sources button to bring out our sources:

![Show Sources](https://NiceRa.in/rain/sharex/screenshots/UE4Editor_2019-01-06_20-52-24.png)

7. In the bottom right corner of our plugin content, we need to click View Options and check the box :ballot_box_with_check: Show Plugin Content. This allows us to add a UW Blueprint to our plugin's content folder.

![Show Plugin Content](https://nicera.in/rain/sharex/screenshots/2019-01-06_20-53-30.png)

8. Inside your plugin's content folder (if you don't see it, did you restart after step 5?), create a new User Interface->Widget Blueprint and name it **BP_SlateWrapper**

![New Widget BP](https://NiceRa.in/rain/sharex/screenshots/UE4Editor_2019-01-06_20-58-30.png)

Note: This is the blueprint that will wrap the slate that we will user for our interface.

9. Navigate to File->New C++ Class with a parent class of Blueprint Function Library

![New C++ Class](https://NiceRa.in/rain/sharex/screenshots/2019-01-06_20-59-47.png)

![BP Function Library](https://NiceRa.in/rain/sharex/screenshots/UE4Editor_2019-01-06_21-00-22.png)

:exclamation: **Make sure to select the plugin to add the C++ class to**

![Plugin Name](https://NiceRa.in/rain/sharex/screenshots/2019-01-06_21-02-02.png)


Note: It will most likely fail to compile the module with the editor open. This is not a big deal, close the editor for now and attempt to Build inside of VS2017


10. Inside of Visual Studios 2017, navigate to your plugin's Build.cs file. You will need to add UMG as a Private Dependency Module:

![UMG Dependency](https://NiceRa.in/rain/sharex/screenshots/devenv_2019-01-06_21-04-19.png)

11. Open your plugin's .uplugin file and change the Module's type from Editor to Developer (in order for UMG to be loaded)

![Developer Flag](https://NiceRa.in/rain/sharex/screenshots/devenv_2019-01-06_21-06-24.png)

12. Within your plugin folder, open YourPlugin.h and add the following to your class's private declaration:

```
//Used to load our userWidget blueprint (which is a slate wrapper)
	void LoadUserWidget();
  
//Stored slate widget that was loaded
	class UUserWidget* CreatedWidget;
```

13. In the YourPlugin.cpp file, add the following headers:

```
//Include LoadObject
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

//Include UserWidget
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
```

14. In the YourPlugin.cpp file, add the following function:

```
//This function does the magic of loading the entire widget from disk into an object.
//Once we've loaded it, we then create a widget and store it to replace our slate UI with.
void FWrapperHelperTempateModule::LoadUserWidget()
{
	//This path is contained within the WrapperHelperTemplate Content folder
	UClass* LoadedWidget = LoadObject<UClass>(NULL, TEXT("/YOUR_PLUGIN_NAME/BP_SlateWrapper.BP_SlateWrapper_C"), NULL, LOAD_None, NULL);
	if (LoadedWidget)
	{
		//If successfully loaded, store it for later use.
		CreatedWidget = CreateWidget(GEditor->GetEditorWorldContext().World(), LoadedWidget);
	}
}
```

:exclamation: Make sure to change FWrapperHelperTempateModule to your module's class name.

:exclamation::exclamation: Make sure to change YOUR_PLUGIN_NAME in the path to successfully load the Blueprint.

15. Rewrite OnSpawnPluginTab's definition to the following:

```
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
		//Store our widget
		TSharedRef<SWidget> UserSlateWidget = CreatedWidget->TakeWidget();

		//Assign our widget to our tab's content
		NewDockTab->SetContent(UserSlateWidget);
	}

	//Return our tab
	return NewDockTab;
```

16. Close the editor, recompile, and your user widget should be loaded within the plugin's window (the plugin's window will be blank unless you added something to UMG).


17. You may now write C++ functions within the blueprint function library and call them from your plugin's user widget!

## Limitations

1. Every time you recompile the User Widget, your window will go blank. This is due to the fact that the pointer to the slate module (wrapped by your widget) is no longer valid. In order to see changes, close the tab and open it again. The reason we don't auto-reload is answered below:

2. This wrapper doesn't pass itself back to the user widget. This means the blueprint is unable to call functions associated with the C++ object (I feel that's a good thing). Because of this, the blueprint is not able to tell the wrapper plugin to reload the window (or call functions on it). This is the primary reason that we use a static blueprint function libary to handle all of our functions.

3. Centered layouts that resize in UMG are quit difficult to create. The UMG of the example plugin has a layout that auto-centers with buttons and a title (regardless of window size).

## Authors

* **Walker 'Nice Rain' Twyman**
