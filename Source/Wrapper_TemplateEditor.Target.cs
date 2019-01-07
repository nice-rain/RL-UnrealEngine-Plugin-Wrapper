// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Wrapper_TemplateEditorTarget : TargetRules
{
	public Wrapper_TemplateEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Wrapper_Template" } );
	}
}
