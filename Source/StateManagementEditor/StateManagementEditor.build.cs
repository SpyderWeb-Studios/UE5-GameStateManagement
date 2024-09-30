using UnrealBuildTool;
 
public class StateManagementEditor : ModuleRules
{
	public StateManagementEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "UnrealEd", "BlueprintGraph", "KismetCompiler", "GameStateManagement"
		});
 
		PublicIncludePaths.AddRange(new string[] {"StateManagementEditor/Public"});
		PrivateIncludePaths.AddRange(new string[] {"StateManagementEditor/Private"});
	}
}