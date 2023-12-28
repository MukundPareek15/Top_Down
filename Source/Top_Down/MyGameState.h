//MyGameState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GenericPlatform/GenericPlatform.h"
#include "MyGameState.generated.h"

UCLASS()
class TOP_DOWN_API AMyGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AMyGameState();
    // Function to update the game timer
    void UpdateGameTimer();

    UPROPERTY(ReplicatedUsing = OnRep_TotalHits)
    uint16 TotalHits;

    UFUNCTION()
    void OnRep_TotalHits();
protected:
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Overridden to handle replication of properties
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    // Timer handle for the game timer
    FTimerHandle GameTimerHandle;

public:
    // The overall game timer (in seconds)
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    int32 GameTimer;

    // Add more replicated game state properties as needed

    void PlayerHit();

    uint16 GetTotalHits() { return TotalHits; }
};
