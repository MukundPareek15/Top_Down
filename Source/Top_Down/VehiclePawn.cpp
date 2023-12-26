//VehiclePaawn.cpp

#include "VehiclePawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "Projectile.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"  // Include BoxComponent header

AVehiclePawn::AVehiclePawn()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetReplicateMovement(true);

    // Replace CapsuleComponent with BoxComponent
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    VehicleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleMesh"));
    VehicleMesh->SetupAttachment(CollisionBox);  // Attach to the CollisionBox

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(CollisionBox);
    SpringArm->TargetArmLength = 600.0f;
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

    ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
    ArrowComponent->SetupAttachment(CollisionBox);
}

void AVehiclePawn::BeginPlay()
{
    Super::BeginPlay();

    CurrentDestination = GetActorLocation();
}

void AVehiclePawn::MoveToLocation(const FVector& DestLocation)
{
    CurrentDestination = DestLocation;

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
    {
        PlayerController->ProjectWorldLocationToScreen(DestLocation, CurrentDestinationScreen);
    }
}

void AVehiclePawn::Shoot()
{
    if (ProjectileClass != nullptr)
    {
        FVector ForwardVector = ArrowComponent->GetForwardVector();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        //SpawnParams.Instigator = Instigator;

        AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
            ProjectileClass,
            ArrowComponent->GetComponentLocation(),
            ForwardVector.Rotation(),
            SpawnParams
        );

        if (Projectile)
        {
            // Optionally, you can set additional properties on the projectile
            // For example, you might want to set the owner or replicate it
            // Projectile->SetOwner(this);
            // Projectile->SetReplicates(true);
        }
    }
}
