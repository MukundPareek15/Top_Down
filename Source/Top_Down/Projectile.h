// Projectile.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

// Forward Declarations
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()

class TOP_DOWN_API AProjectile : public AActor
{
    GENERATED_BODY()

public:
    AProjectile();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:

    FVector InitialVelocity;

public:
    // Components
    //UPROPERTY(EditAnywhere, Category = "Components")
  // class UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(EditAnywhere, Category = "Components")
    class UProjectileMovementComponent* ProjectileComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* HitSphere;

    // Function to handle collision
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    // Function that initializes the projectile's velocity in the shoot direction.
    void FireInDirection(const FVector& ShootDirection);

    // Called every frame
    void LaunchProjectile(float Speed);

    // Function that handles firing projectiles.
    //UFUNCTION()

    /** How much to change health by when hitting another player. */
    UPROPERTY(EditAnywhere)
    float ScoreDelta;


    AActor* LineTraceSingle(FVector Start, FVector End);
    AActor* LineTraceSingle(FVector Start, FVector End, bool ShowDebugLine);
};
