// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPS_GAS : ModuleRules
{
    public TPS_GAS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        //GAS
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate",
            "SlateCore",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks"
        });

        //MP
        PrivateDependencyModuleNames.AddRange(new string[] {
            "OnlineSubsystem",
            "OnlineSubsystemSteam"
        });

        //additional
        PrivateDependencyModuleNames.AddRange(new string[] {
            "AIModule",
            "UMG",
            "MovieScene",
            "ChaosSolverEngine",
            "FieldSystemEngine",
            "GeometryCollectionEngine",
            "Niagara"
        });


    }
}
