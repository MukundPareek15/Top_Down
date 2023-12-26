//Top_DownGameMode.cpp

#include "Top_DownGameMode.h"
#include "Top_DownPlayerController.h"
#include "Top_DownCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATop_DownGameMode::ATop_DownGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATop_DownPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}