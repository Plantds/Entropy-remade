// Copyright (c) 2020 Tension Graphics AB

using UnrealBuildTool;
using System.Collections.Generic;

public class EntropyEditorTarget : TargetRules
{
	public EntropyEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Entropy" } );
	}
}
