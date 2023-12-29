//MyGameModeBase.cpp

#include "MyGameModeBase.h"
#include "MyPlayerController.h" // PlayerController class
#include "VehiclePawn.h" // VehiclePawn class
#include "VehicleCharacter.h"
#include "SpawnPoint.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "MyGameState.h"
#include "MyHUD.h"

AMyGameModeBase::AMyGameModeBase()
{
    // Set default player controller class
    PlayerControllerClass = AMyPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/C++BluePrints/BPVehicleCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/C++BluePrints/BPMyPlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	DefaultSpawnLocation = FVector(960.0f, 860.0f, 200.0f);
	RespawnTime = 1.0f;
}

void AMyGameModeBase::PlayerHit()
{
	if (AMyGameState* GS = GetGameState< AMyGameState>())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player Hit Game Mode"));
		GS->PlayerHit();
	}
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UClass* SpanPointClass = ASpawnPoint::StaticClass();
	for (TActorIterator<AActor> Actor (GetWorld(), SpanPointClass); Actor; ++Actor)
	{
		SpawnPoints.Add(Cast<ASpawnPoint>(*Actor));
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawn Points: %d"), SpawnPoints.Num());
}

void AMyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (AController* Controller = Cast< AController>(NewPlayer))
	{
		Spawn(Controller);
	}
}

ASpawnPoint* AMyGameModeBase::GetSpawnPoint()
{
	for (int32 i = 0; i < SpawnPoints.Num(); ++i)
	{
		int32 Slot = FMath::RandRange(0, SpawnPoints.Num() - 1);
		if (SpawnPoints[Slot])
			return SpawnPoints[Slot];
	}
	return nullptr;
}

void AMyGameModeBase::Spawn(AController* Controller)
{
	if (ASpawnPoint* SpawnPoint = GetSpawnPoint())
	{
		FVector Location = SpawnPoint->GetActorLocation();
		FRotator Rotation = SpawnPoint->GetActorRotation();
		if (APawn* Pawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, Location, Rotation))
		{
			Controller->Possess(Pawn);
		}
	}
	else
	{
		FVector Location = DefaultSpawnLocation;
		FRotator Rotation = FRotator::ZeroRotator;
		if (APawn* Pawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, Location, Rotation))
		{
			Controller->Possess(Pawn);
		}
	}
}

void AMyGameModeBase::Respawn(AController* Controller)
{
	if (Controller)
	{
		if (HasAuthority())
		{
			//UE_LOG(LogTemp, Warning, TEXT("IN RESPAWN"));

			FTimerDelegate RespawnDele;
			RespawnDele.BindUFunction(this, FName("Spawn"), Controller);
			FTimerHandle RespawnHandle;
			GetWorld()->GetTimerManager().SetTimer(RespawnHandle, RespawnDele, RespawnTime, false);
		}
	}
}

	