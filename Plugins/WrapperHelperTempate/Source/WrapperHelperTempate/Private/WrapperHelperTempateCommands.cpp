// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "WrapperHelperTempateCommands.h"

#define LOCTEXT_NAMESPACE "FWrapperHelperTempateModule"

void FWrapperHelperTempateCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "WrapperHelperTempate", "Bring up WrapperHelperTempate window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
