#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BLPlayerState.generated.h"

/**
 * Team designation for 5v5 matches.
 */
UENUM(BlueprintType)
enum class EBLTeam : uint8
{
    None = 0    UMETA(DisplayName = "Unassigned"),
    Home = 1    UMETA(DisplayName = "Home"),
    Away = 2    UMETA(DisplayName = "Away")
};

/**
 * ABLPlayerState
 *
 * Per-player replicated state for a streetball match: team assignment and
 * individual scoring. Server-authoritative; clients receive updates via
 * replication.
 */
UCLASS()
class BLACKTOPLEGENDS_API ABLPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    ABLPlayerState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** Which team this player is on. Set by the game mode on login. */
    UPROPERTY(ReplicatedUsing = OnRep_Team, BlueprintReadOnly, Category = "BL|Team")
    EBLTeam Team = EBLTeam::None;

    /** Points this player has personally scored. */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "BL|Score")
    int32 PointsScored = 0;

    /** Server-only: assign this player to a team. */
    void SetTeam(EBLTeam NewTeam);

    /** Server-only: add to this player's personal score. */
    void AddPoints(int32 Points);

protected:
    UFUNCTION()
    void OnRep_Team();
};
