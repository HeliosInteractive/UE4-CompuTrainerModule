Setup (.dll placement):

You must aquire the .dlls from the computrainer SDK, we are not allowed to redistrubte.

1. racermate.h -> "CompuTrainerModule\ThirdParty\RacerMate\Include\RacerMate"
2. freetype6.dll, pcre3.dll, racermate.dll, racermate.lib, zlib1.dll -> "CompuTrainerModule\ThirdParty\RacerMate\Lib"

Installing the CompuTrainerModule:
1. Copy the CompuTrainerModule folder into your project's Source folder.
2. Open "*.Target.cs" and "*Editor.Target.cs" and add the module to the OutExtraModuleNames.
3. Add the module as runtime during the default loading phase to the .uproject file:
{
	"Name": "CompuTrainerModule",
	"Type": "Runtime",
	"LoadingPhase": "Default"
},
4. Recompile
5. Open the editor and use!

Building Projects:
1. Copy all .dll files to the into the build folder beside the .exe.