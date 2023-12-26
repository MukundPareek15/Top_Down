//MyHUD.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "MyHUD.generated.h"

UCLASS()
class TOP_DOWN_API UMyHUD : public UUserWidget
{
    GENERATED_BODY()

public:

    //UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void UpdateScore(int32 NewScore);

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UTextBlock* MyScore;
};
