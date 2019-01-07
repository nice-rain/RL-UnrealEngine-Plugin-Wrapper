// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Wrapper_TemplateTarget : TargetRules
{
	public Wrapper_TemplateTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Wrapper_Template" } );
	}
}
