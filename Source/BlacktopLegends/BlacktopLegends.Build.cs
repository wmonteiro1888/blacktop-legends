using UnrealBuildTool;

public class BlacktopLegends : ModuleRules
{
    public BlacktopLegends(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            // Online session host / find / join support (desktop matchmaking).
            "OnlineSubsystem",
            "OnlineSubsystemUtils"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Pull in the LAN/NULL backend so sessions work with no external service.
        // Configure Steam (or another provider) in Config/DefaultEngine.ini for
        // online play; this only guarantees the NULL subsystem is available.
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
