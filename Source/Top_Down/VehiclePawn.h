//VehiclePawn.h

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Projectile.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Pawn.h"
#include "VehiclePawn.generated.h"

UCLASS()
class TOP_DOWN_API AVehiclePawn : public APawn
{
    GENERATED_BODY()

public:
    AVehiclePawn();

    // Function to move to a location
    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void MoveToLocation(const FVector& DestLocation);

    // Function to handle shooting
    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void Shoot();

protected:
    virtual void BeginPlay() override;

public:
    // Components
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class USkeletalMeshComponent* VehicleMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class UCameraComponent* Camera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class USpringArmComponent* SpringArm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class UBoxComponent* CollisionBox;  // Replace CapsuleComponent with BoxComponent

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class UArrowComponent* ArrowComponent;

    // Projectile class to spawn
    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TSubclassOf<class AProjectile> ProjectileClass;

private:
    FVector CurrentDestination;
    FVector2D CurrentDestinationScreen;
};