// Projectile.cpp

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "VehiclePawn.h" // Include your VehiclePawn header
#include "MyPlayerState.h" // Include your MyPlayerState header
#include "VehicleCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "VehicleCharacter.h"
#include "Net/UnrealNetwork.h"
#include "MyGameModeBase.h"

AProjectile::AProjectile()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = false;
    
    HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    HitSphere->SetSimulatePhysics(true);
    HitSphere->SetNotifyRigidBodyCollision(true);
    HitSphere->BodyInstance.SetCollisionProfileName("BlockAllDynamic");

    RootComponent = HitSphere;

    // Create and configure the static mesh component
    //ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));

    // Create and configure the projectile movement component
    ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
    //ProjectileComponent->UpdatedComponent = ProjectileMesh;
    ProjectileComponent->InitialSpeed = 8000.f;
    ProjectileComponent->MaxSpeed = 14000.f;
    ProjectileComponent->bRotationFollowsVelocity = true;
    ProjectileComponent->bShouldBounce = false;
    ProjectileComponent->bAutoActivate = false;

    // Set up collision handling
    //ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    ScoreDelta = +1.f;
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    SetReplicates(true);
    HitSphere->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    //ProjectileComponent->Velocity = InitialVelocity;
}

void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

/*void AProjectile::FireInDirection(const FVector& ShootDirection)
{
    InitialVelocity = ShootDirection * ProjectileComponent->InitialSpeed;
}*/

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)  && OtherComp->IsSimulatingPhysics())
    {
        UE_LOG(LogTemp, Warning, TEXT("HIT A PHYSICS OBJ"));
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
        {
            //AVehicleCharacter* OtherVehicle = Cast<AVehicleCharacter>(OtherActor);
            //if (OtherVehicle)
            //{
            //    OtherVehicle->Die();
            //    //and add the count of the kill in your Hud

            //    /*AVehicleCharacter* Target = Cast<AVehicleCharacter>(OtherActor);
            //    AVehicleCharacter* Shooter = GetInstigator<AVehicleCharacter>();
            //    if (Target && Target != Shooter && Target->GetLocalRole() == ROLE_Authority)
            //    {
            //        Target->UpdateScore(ScoreDelta);
            //    }*/
            //}
            //else
            //{
            //    //return NULL;
            //}
        }
    }
    else if (HasAuthority())
    {
        if (AVehicleCharacter* OtherVehicle = Cast<AVehicleCharacter>(OtherActor))
        {
            if (AMyGameModeBase* GM = GetWorld()->GetAuthGameMode<AMyGameModeBase>())
            {
                GM->PlayerHit();
                if (AVehicleCharacter* ShootingVehicle = Cast<AVehicleCharacter>(GetOwner()))
                {
                    if (ShootingVehicle)
                    {
                        if (AMyPlayerState* PS = ShootingVehicle->GetPlayerState<AMyPlayerState>())
                        {
                            PS->PlayerHit();
                            if (OtherVehicle)
                            {

                                OtherVehicle->Die();
                            }
                        }
                    }
                }
                else
                {
                    // ShootingVehicle is not valid, handle this situation
                    UE_LOG(LogTemp, Warning, TEXT("ShootingVehicle is not valid."));
                }
            }
        }
    }
}
    
AActor* AProjectile::LineTraceSingle(FVector Start, FVector End)
{
    FHitResult HitResult;
    FCollisionObjectQueryParams CollisionParams;
    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(GetOwner());

    if (GetWorld()->LineTraceSingleByObjectType(
        OUT HitResult,
        Start,
        End,
        CollisionParams,
        CollisionQueryParams
    ))
    {
        return HitResult.GetActor();
    }
    else
    {
        return nullptr;
    }
    
}

AActor* AProjectile::LineTraceSingle(FVector Start, FVector End, bool ShowDebugLine)
{
    AActor* Actor = LineTraceSingle(Start, End);
    if (ShowDebugLine)
    {
        DrawDebugLine(
            GetWorld(),
            Start,
            End,
            FColor::Red,
            false,
            3.0f,
            0,
            2.0f
        );
    }

    return Actor;
}

// Function that initializes the projectile's velocity in the shoot direction.
void AProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileComponent->Velocity = ShootDirection * ProjectileComponent->InitialSpeed;
}

void AProjectile::LaunchProjectile(float Speed) {
    // Get the vector to fire the projectile in based on the Actor that spawns the Projectile and multiply it by the Speed parameter passed by the Actor spawning the projectile
    ProjectileComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
    // Tell the projectile to move
    ProjectileComponent->Activate();
}