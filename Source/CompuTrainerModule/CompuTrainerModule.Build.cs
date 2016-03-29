// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class CompuTrainerModule : ModuleRules
{
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "ThirdParty/")); }
    }

    private string ModulePath
    {
        get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
    }

	public CompuTrainerModule(TargetInfo Target)
	{
		
		// ... add public include paths required here ...
		PublicIncludePaths.AddRange(new string[] {"CompuTrainerModule/Public"});
				
		
	    // ... add other private include paths required here ...
		PrivateIncludePaths.AddRange(new string[] {
            "CompuTrainerModule/Private",
            Path.Combine(ThirdPartyPath, "RacerMate", "Include")
        });
			
		// ... add other public dependencies that you statically link with here ...
		PublicDependencyModuleNames.AddRange(new string[] {
				"Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "Slate",
                "SlateCore"
			});
			
		
		// ... add private dependencies that you statically link with here ...	
		PrivateDependencyModuleNames.AddRange(new string[]{});
		
		// ... add any modules that your module loads dynamically here ...
		DynamicallyLoadedModuleNames.AddRange(new string[]{});

        bool isLibrarySupported = false;
        if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            string LibrariesPath = Path.Combine(ThirdPartyPath, "RacerMate", "Lib");
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "racermate.lib"));
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "RacerMate", "Include"));

            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(LibrariesPath, "zlib1.dll")));
            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(LibrariesPath, "freetype6.dll")));
            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(LibrariesPath, "pcre3.dll")));
            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(LibrariesPath, "racermate.dll")));
            isLibrarySupported = true;
        }

        Definitions.Add(string.Format("WITH_COMPUTRAINER_BINDING={0}", isLibrarySupported ? 1 : 0));
        
    }
}
