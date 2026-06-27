#include "BLGameMode.h"
#include "BLPlayerState.h"
#include "GameFramework/PlayerController.h"

ABLGameMode::ABLGameMode()
{
    PlayerStateClass = ABLPlayerState::StaticClass();
}

void ABLGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    AssignPlayerToTeam(NewPlayer);
}

void ABLGameMode::Logout(AController* Exiting)
{
    if (const APlayerController* PC = Cast<APlayerController>(Exiting))
    {
        if (const ABLPlayerState* PS = PC->GetPlayerState<ABLPlayerState>())
        {
            if (PS->Team == EBLTeam::Home)
            {
                HomeCount = FMath::Max(0, HomeCount - 1);
            }
            else if (PS->Team == EBLTeam::Away)
            {
                AwayCount = FMath::Max(0, AwayCount - 1);
            }
        }
    }

    Super::Logout(Exiting);
}

void ABLGameMode::AssignPlayerToTeam(APlayerController* NewPlayer)
{
    if (!NewPlayer)
    {
        return;
    }

    ABLPlayerState* PS = NewPlayer->GetPlayerState<ABLPlayerState>();
    if (!PS)
    {
        return;
    }

    // Fill the smaller team first; ties go to Home.
    const EBLTeam Assigned = (HomeCount <= AwayCount) ? EBLTeam::Home : EBLTeam::Away;
    PS->SetTeam(Assigned);

    if (Assigned == EBLTeam::Home)
    {
        ++HomeCount;
    }
    else
    {
        ++AwayCount;
    }

    UE_LOG(LogTemp, Log, TEXT("[BL] %s joined team %s (Home %d / Away %d)"),
        *PS->GetPlayerName(),
        Assigned == EBLTeam::Home ? TEXT("Home") : TEXT("Away"),
        HomeCount, AwayCount);
}

void ABLGameMode::AddTeamScore(uint8 Team, int32 Points)
{
    if (static_cast<EBLTeam>(Team) == EBLTeam::Home)
    {
        HomeScore += Points;
    }
    else if (static_cast<EBLTeam>(Team) == EBLTeam::Away)
    {
        AwayScore += Points;
    }

    UE_LOG(LogTemp, Log, TEXT("[BL] Score — Home %d : %d Away"), HomeScore, AwayScore);
}
