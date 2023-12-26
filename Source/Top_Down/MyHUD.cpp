//MyHUD.cpp

#include "MyHUD.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void UMyHUD::UpdateScore(int32 NewScore)
{
	if (MyScore)
	{
		MyScore->SetText(FText::AsNumber(NewScore));
	}
        
}
