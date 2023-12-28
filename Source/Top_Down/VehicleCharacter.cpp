// VehicleCharacter.cpp


#include "VehicleCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
//#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "MyHUD.h"
#include "Projectile.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "VehicleCharacter.h"
#include "MyGameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h" 
#include "MyPlayerController.h"

// Sets default values
AVehicleCharacter::AVehicleCharacter()
{
	// Set size for player capsule
	//GetCapsuleComponent()->DestroyComponent();
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetBoxExtent(FVector(150.0f, 100.0f, 96.0f));  // Set the box extent as needed


	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a arrow...
	//ProjectileRoot = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileRoot"));
	//ProjectileRoot->SetupAttachment(RootComponent);

	//HUD
	PlayerHUDClass = nullptr;
	PlayerHud = nullptr;
	Score = 0.0f;
	MaxScore = 250.0f;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//ProjectileComp = CreateDefaultSubobject<UProjectile>("ProjectileComponent");

}

// Called when the game starts or when spawned
void AVehicleCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	
	if (IsLocallyControlled() && PlayerHUDClass)
	{
		AMyPlayerController* MPc = GetController<AMyPlayerController>();
		check(MPc);
		PlayerHud = CreateWidget<UMyHUD>(MPc, PlayerHUDClass);
		check(PlayerHud);
		PlayerHud->AddToPlayerScreen();
		PlayerHud->UpdateScore(Score);
	}
	//GetWorld()->SpawnActor<AProjectile>(ProjectileActor, GetActorLocation(), GetActorRotation());
	
}

void AVehicleCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AVehicleCharacter, Score, COND_OwnerOnly);
	// Replicate to everyone
	//DOREPLIFETIME(AVehicleCharacter, Fire);
}

// Called every frame
void AVehicleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVehicleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVehicleCharacter::Fire()
{
    if (!ensure(ProjectileRoot)) { return; }

    // Spawn the projectile at the specified location and rotation
    auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileActor, ProjectileRoot->GetSocketLocation(FName("Projectile")), ProjectileRoot->GetSocketRotation(FName("Projectile")));
    
    if (Projectile)
    {
        // Set the projectile's owner to be this vehicle character
        Projectile->SetOwner(this);

		UE_LOG(LogTemp, Warning, TEXT("Get Owner"))
		//Projectile->SetReplicates(true); 

        // Launch the projectile
        Projectile->LaunchProjectile(LaunchSpeed);

        // If this is the player character, call the server fire function
        if (AVehicleCharacter* Player = Cast<AVehicleCharacter>(this))
        {
            ServerFire();
        }
    }
}


bool AVehicleCharacter::ServerFire_Validate()
{
	return true;
}

void AVehicleCharacter::ServerFire_Implementation()
{
	if (HasAuthority())
	{ 
		if (!ensure(ProjectileRoot)) { return; }

		// Spawn the projectile at the specified location and rotation
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileActor, ProjectileRoot->GetSocketLocation(FName("Projectile")), ProjectileRoot->GetSocketRotation(FName("Projectile")));

		if (Projectile)
		{
			// Set the projectile's owner to be this vehicle character
			Projectile->SetOwner(this);

			// Launch the projectile
			Projectile->LaunchProjectile(LaunchSpeed);

			// If this is the player character, call the server fire function
			if (AVehicleCharacter* Player = Cast<AVehicleCharacter>(this))
			{
				//ServerFire();
			}
		}
	}
}

float AVehicleCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{

	}
	
	return ActualDamage;
}

void AVehicleCharacter::AddScore(int32 Points)
{
	PlayerScore += Points;
	if (PlayerHud)
	{
		PlayerHud->UpdateScore(PlayerScore);
	}
}

void AVehicleCharacter::CallDestroy()
{
	if (HasAuthority())
		Destroy();
}

void AVehicleCharacter::OnRepScore()
{
	if (PlayerHud)
	{
		PlayerHud->UpdateScore(Score);
	}
}


void AVehicleCharacter::UpdateScore(float ScoreDelta)
{
	Score = FMath::Clamp(Score + ScoreDelta, 0.f, MaxScore);
}

/*void AVehicleCharacter::UpdateHUDScore(int32 NewScore)
{
	if (PlayerHud)
	{
		PlayerHud->UpdateScore(NewScore);
	}
}*/

void AVehicleCharacter::Die()
{
	if (HasAuthority())
	{
		/*AController* InstigatorController = GetInstigatorController();
		AVehicleCharacter* InstigatorVehicle = Cast<AVehicleCharacter>(InstigatorController->GetPawn());
		if (InstigatorVehicle)
		{
			InstigatorVehicle->AddScore(+1); // Award 1 points for destruction
			// Update HUD Here: InstigatorVehicle->UpdateHUDScore();
		}*/
		MultiDie();

		AGameModeBase* GM = GetWorld()->GetAuthGameMode();
		if (AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(GM))
		{
			GameMode->Respawn(GetController());
		}
		//Start Timer To remove Actor from world

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AVehicleCharacter has died"));
		}
		GetWorld()->GetTimerManager().SetTimer(DestroyHandle, this, &AVehicleCharacter::CallDestroy, 0.5f, false);
	}
}

bool AVehicleCharacter::MultiDie_Validate()
{
	return true;
}

void AVehicleCharacter::MultiDie_Implementation()
{
	//ShutDown
	GetCharacterMovement()->DisableMovement();
	this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	this->GetMesh()->SetAllBodiesSimulatePhysics(true);
}
