// MyPlayerState.cpp
#include "MyPlayerState.h"
#include "VehicleCharacter.h"
#include "MyHUD.h"
#include "Net/UnrealNetwork.h"
#include "MyGameState.h"

AMyPlayerState::AMyPlayerState()
{
    //MyScore = 0;
}

<<<<<<< HEAD
//void AMyPlayerState::AddScore(float ScoreDelta)
//{
//    if (HasAuthority())
//    {
//        MyScore += ScoreDelta;
//        // Optional: Notify the clients about the score update, if needed
//    }
//}
//
//void AMyPlayerState::OnRep_MyScore()
//{
//    AVehicleCharacter* VehicleChar = Cast<AVehicleCharacter>(GetPawn());
//    if (VehicleChar)
//    {
//       // VehicleChar->UpdateHUDScore(MyScore);
//    }
//}
=======
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
>>>>>>> parent of 1445803 (Just some changes)


void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Ensure the MyScore is replicated to any client connected to the game
    DOREPLIFETIME (AMyPlayerState, TotalHits);
}

void AMyPlayerState::OnRep_TotalHits()
{
    if (AMyGameState* GS = GetWorld()->GetGameState<AMyGameState>())
    {
        uint16 GSHits = GS->GetTotalHits();
            UE_LOG(LogTemp, Warning, TEXT("%d/%d of the total hits"), TotalHits, GSHits);
    }
}

void AMyPlayerState::PlayerHit()
{
    //UE_LOG(LogTemp, Warning, TEXT("Player Hit Player State"));
    ++TotalHits;
    //UE_LOG(LogTemp, Warning, TEXT("Server: PlayerHits: %d"), TotalHits);
}
