Setup
=======
**Note:** _You must aquire the .dlls from the computrainer SDK, we are not allowed to redistrubte._

1. racermate.h -> "CompuTrainerModule\ThirdParty\RacerMate\Include\RacerMate"
2. freetype6.dll, pcre3.dll, racermate.dll, racermate.lib, zlib1.dll -> "CompuTrainerModule\ThirdParty\RacerMate\Lib"

Installing the CompuTrainerModule:

1. Copy the CompuTrainerModule folder into your project's Source folder.  
2. Open "*.Target.cs" and "*Editor.Target.cs" and add the module to the OutExtraModuleNames.  
3. Add the module as runtime during the default loading phase to the .uproject file:  
`{  
	"Name": "CompuTrainerModule",  
	"Type": "Runtime",  
	"LoadingPhase": "Default"  
},`  
4. Recompile
5. Open the editor and use!

Usage
=======
Setting up the game instance:
<img src="https://raw.githubusercontent.com/HeliosInteractive/UE4-CompuTrainerModule/master/GameInstance.png">  
Setting up the player pawn:
<img src="https://raw.githubusercontent.com/HeliosInteractive/UE4-CompuTrainerModule/master/PlayerPawn.png">  
Adding debug controls to the player pawn:
<img src="https://raw.githubusercontent.com/HeliosInteractive/UE4-CompuTrainerModule/master/PlayerPawn_EditorMovement.png">

At this point you're ready to start testing in editor and making builds which respond to the CompuTrainer's output! Don't forget to also check out the other blueprint functions found in the Compu Trainer category.

Shipping:
=======
Copy all .dll files to the into the build folder beside the .exe as they are needed at runtime and unreal does not copy them over properly at the time of this posting.
