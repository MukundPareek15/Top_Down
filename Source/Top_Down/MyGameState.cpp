//MyGameState.cpp

#include "MyGameState.h"
#include "Net/UnrealNetwork.h"

AMyGameState::AMyGameState()
{
    // Initialize the game timer
    GameTimer = 0;
}

void AMyGameState::BeginPlay()
{
    Super::BeginPlay();

    if (GetLocalRole() == ROLE_Authority)
    {
        // Set up a repeating timer to update the game timer
        GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &AMyGameState::UpdateGameTimer, 1.0f, true);
    }
}

void AMyGameState::UpdateGameTimer()
{
    GameTimer++;
    // Implement additional game timer logic as needed
}

void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Ensure the GameTimer is replicated to any client connected to the game
    DOREPLIFETIME(AMyGameState, GameTimer);
}
