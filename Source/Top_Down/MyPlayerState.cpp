// MyPlayerState.cpp
#include "MyPlayerState.h"
#include "VehicleCharacter.h"
#include "MyHUD.h"
#include <Net/UnrealNetwork.h>

AMyPlayerState::AMyPlayerState()
{
    MyScore = 0;
}

/*void AMyPlayerState::AddScore(float ScoreDelta)
{
    if (HasAuthority())
    {
        //MyScore += ScoreDelta;
        // Optional: Notify the clients about the score update, if needed
    }
}*/

/*void AMyPlayerState::OnRep_MyScore()
{
    AVehicleCharacter* VehicleChar = Cast<AVehicleCharacter>(GetPawn());
    if (VehicleChar)
    {
       // VehicleChar->UpdateHUDScore(MyScore);
    }
}*/


void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Ensure the MyScore is replicated to any client connected to the game
    DOREPLIFETIME_CONDITION(AMyPlayerState, MyScore, COND_None);
}
