// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPS_Project : ModuleRules
{
	public TPS_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"TPS_Project",
			"TPS_Project/Variant_Platforming",
			"TPS_Project/Variant_Platforming/Animation",
			"TPS_Project/Variant_Combat",
			"TPS_Project/Variant_Combat/AI",
			"TPS_Project/Variant_Combat/Animation",
			"TPS_Project/Variant_Combat/Gameplay",
			"TPS_Project/Variant_Combat/Interfaces",
			"TPS_Project/Variant_Combat/UI",
			"TPS_Project/Variant_SideScrolling",
			"TPS_Project/Variant_SideScrolling/AI",
			"TPS_Project/Variant_SideScrolling/Gameplay",
			"TPS_Project/Variant_SideScrolling/Interfaces",
			"TPS_Project/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
