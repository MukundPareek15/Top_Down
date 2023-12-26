//MyGameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

UCLASS()
class TOP_DOWN_API AMyGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMyGameModeBase();

protected:
    TArray<class ASpawnPoint*> SpawnPoints;
    FVector DefaultSpawnLocation;

    UPROPERTY(EditAnywhere, Category = "S|Respawn")
    float RespawnTime;

    class ASpawnPoint* GetSpawnPoint();

    UFUNCTION()
    void Spawn(AController* Controller);

    virtual void PostLogin(APlayerController* NewPlayer) override;
public:
    void Respawn(AController* Controller);

    virtual void BeginPlay() override;
};
