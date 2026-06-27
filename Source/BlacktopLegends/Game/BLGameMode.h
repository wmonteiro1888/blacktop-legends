#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BLGameMode.generated.h"

/**
 * ABLGameMode
 *
 * Server-authoritative match rules for 5v5 streetball. Balances incoming
 * players across Home/Away as they join a hosted session and tracks the
 * team scoreline.
 */
UCLASS()
class BLACKTOPLEGENDS_API ABLGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABLGameMode();

    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;

    /** Players per team for a full game (5v5). */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Match")
    int32 PlayersPerTeam = 5;

    /** Server-only: credit a team with points and update the scoreline. */
    UFUNCTION(BlueprintCallable, Category = "BL|Match")
    void AddTeamScore(uint8 Team, int32 Points);

protected:
    /** Assign a freshly logged-in player to the team with fewer members. */
    void AssignPlayerToTeam(APlayerController* NewPlayer);

private:
    int32 HomeCount = 0;
    int32 AwayCount = 0;

    int32 HomeScore = 0;
    int32 AwayScore = 0;
};
