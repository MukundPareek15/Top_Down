// VehicleCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Projectile.h"
#include "VehicleCharacter.generated.h"


UCLASS(Blueprintable)
class TOP_DOWN_API AVehicleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVehicleCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Box component for the character's collision */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ProjectileRoot;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 8000;

private:
	UPROPERTY(VisibleAnywhere, Category = "Score")
	int32 PlayerScore = 0;

	UPROPERTY(EditAnywhere)
	float MaxScore;

	/** Current Score of player. */
	UPROPERTY(ReplicatedUsing = OnRepScore)
	float Score;

	/** Callback when Score is updated via replication. */
	UFUNCTION()
	void OnRepScore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	//class UProjectile* ProjectileComp;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Fire();
	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<class AProjectile> ProjectileActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMyHUD> PlayerHUDClass;

	UPROPERTY()
	class UMyHUD* PlayerHud;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();
	bool ServerFire_Validate();
	void ServerFire_Implementation();
	// Gun muzzle offset from Vehicle location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//UPROPERTY(BlueprintReadOnly, Category = "UI")
	void AddScore(int32 Points);

	void Die();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MultiDie();
	bool MultiDie_Validate();
	void MultiDie_Implementation();

	FTimerHandle DestroyHandle;

	void CallDestroy();

	//UPROPERTY(EditAnywhere)
	//float ScoreDelta;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateScore(float ScoreDelta);
};

