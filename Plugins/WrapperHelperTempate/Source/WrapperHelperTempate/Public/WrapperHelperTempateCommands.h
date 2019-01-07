// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "WrapperHelperTempateStyle.h"

class FWrapperHelperTempateCommands : public TCommands<FWrapperHelperTempateCommands>
{
public:

	FWrapperHelperTempateCommands()
		: TCommands<FWrapperHelperTempateCommands>(TEXT("WrapperHelperTempate"), NSLOCTEXT("Contexts", "WrapperHelperTempate", "WrapperHelperTempate Plugin"), NAME_None, FWrapperHelperTempateStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};