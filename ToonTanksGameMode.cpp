
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"


void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    if (DeadActor == Tank)
    {
        Tank -> HandleDestruction();
        if (ToonTanksPlayerController)
        {
            ToonTanksPlayerController -> SetPlayerEnabledState(false);
        }
        GameOver(false);
    }
    else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower -> HandleDestruction();
        TargetTowers --;
        if (TargetTowers == 0)
        {
            GameOver(true);
        }
    }
}

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();
    HandleGameStart();
}