//MyHUD.cpp

#include "MyHUD.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "MyGameModeBase.h"
#include "MyGameState.h"
#include "MyPlayerState.h"

void UMyHUD::UpdateScore(int32 NewScore)
{
	if (MyScore)
	{
		MyScore->SetText(FText::AsNumber(NewScore));
	}
        
}
