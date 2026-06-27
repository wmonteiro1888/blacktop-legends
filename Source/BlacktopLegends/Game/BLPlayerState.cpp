#include "BLPlayerState.h"
#include "Net/UnrealNetwork.h"

ABLPlayerState::ABLPlayerState()
{
    // PlayerState replicates by default; nothing extra to configure here.
}

void ABLPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABLPlayerState, Team);
    DOREPLIFETIME(ABLPlayerState, PointsScored);
}

void ABLPlayerState::SetTeam(EBLTeam NewTeam)
{
    if (HasAuthority())
    {
        Team = NewTeam;
        OnRep_Team();
    }
}

void ABLPlayerState::AddPoints(int32 Points)
{
    if (HasAuthority())
    {
        PointsScored += Points;
    }
}

void ABLPlayerState::OnRep_Team()
{
    // Hook for UI/visuals (jersey color, scoreboard) to react to team changes.
}
