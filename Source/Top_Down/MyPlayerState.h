// MyPlayerState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

UCLASS()
class TOP_DOWN_API AMyPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AMyPlayerState();

    // Function to add score
    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(float ScoreDelta);

    // Replicated Score property for AMyPlayerState
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
    float MyScore;

    UFUNCTION()
    void OnRep_MyScore();

protected:
    // Replicate Score using DOREPLIFETIME_CONDITION
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

    /*UFUNCTION(Server, Reliable, WithValidation)
    void ServerLowerHealth(float Value);
    bool Server LowerHealth_Validate(float Value);
    void ServerLowerHealth_Implementation(float Value);*/
};
