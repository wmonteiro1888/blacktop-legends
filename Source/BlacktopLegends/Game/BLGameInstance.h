#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "BLGameInstance.generated.h"

/**
 * Blueprint-friendly mirror of a single discovered session.
 *
 * FOnlineSessionSearchResult is not a UStruct, so it cannot cross the
 * Blueprint boundary. We flatten the fields a join menu actually needs and
 * keep the underlying results indexed internally by SearchResultIndex.
 */
USTRUCT(BlueprintType)
struct FBLSessionInfo
{
    GENERATED_BODY()

    /** Index into the last search; pass back to JoinFoundSession(). */
    UPROPERTY(BlueprintReadOnly, Category = "Sessions")
    int32 SearchResultIndex = -1;

    /** Host-advertised lobby name (defaults to the host's player name). */
    UPROPERTY(BlueprintReadOnly, Category = "Sessions")
    FString ServerName;

    /** Measured round-trip latency to the host, in milliseconds. */
    UPROPERTY(BlueprintReadOnly, Category = "Sessions")
    int32 PingMs = 0;

    /** Currently filled player slots. */
    UPROPERTY(BlueprintReadOnly, Category = "Sessions")
    int32 CurrentPlayers = 0;

    /** Total player slots the host opened. */
    UPROPERTY(BlueprintReadOnly, Category = "Sessions")
    int32 MaxPlayers = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBLOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBLOnFindSessionsComplete, const TArray<FBLSessionInfo>&, Results, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBLOnJoinSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBLOnDestroySessionComplete, bool, bWasSuccessful);

/**
 * UBLGameInstance
 *
 * Owns Blacktop Legends' online session lifecycle so a desktop client can
 * host, browse, and join 5v5 matches. Wraps the Online Subsystem session
 * interface (OnlineSubsystemNull/LAN by default, Steam-ready) behind
 * Blueprint-callable entry points and broadcast delegates that the main-menu
 * UI binds to.
 */
UCLASS()
class BLACKTOPLEGENDS_API UBLGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UBLGameInstance();

    virtual void Init() override;

    // ===== Public API (bind these from the desktop menu) =====

    /**
     * Host a new match. Creates a session and, on success, server-travels to
     * MatchMap as a listen server so remote clients can connect.
     *
     * @param NumPublicConnections  Total player slots (10 for full 5v5).
     * @param bIsLAN                 LAN discovery only (true) or online (false).
     * @param MatchMap               Map to open as listen server, e.g. "HarlemCrown".
     */
    UFUNCTION(BlueprintCallable, Category = "Sessions")
    void HostSession(int32 NumPublicConnections = 10, bool bIsLAN = true, const FString& MatchMap = TEXT("HarlemCrown"));

    /**
     * Search for joinable sessions. Results are reported via OnFindSessionsComplete.
     *
     * @param MaxSearchResults  Upper bound on results to return.
     * @param bIsLAN            Search the LAN (true) or online backend (false).
     */
    UFUNCTION(BlueprintCallable, Category = "Sessions")
    void FindSessions(int32 MaxSearchResults = 20, bool bIsLAN = true);

    /**
     * Join a session previously returned by FindSessions. On success the
     * client travels to the host's resolved connect address.
     *
     * @param SearchResultIndex  Index from an FBLSessionInfo entry.
     */
    UFUNCTION(BlueprintCallable, Category = "Sessions")
    void JoinFoundSession(int32 SearchResultIndex);

    /** Leave/tear down the current session (call before returning to menu). */
    UFUNCTION(BlueprintCallable, Category = "Sessions")
    void DestroySession();

    // ===== UI-bindable completion delegates =====

    UPROPERTY(BlueprintAssignable, Category = "Sessions")
    FBLOnCreateSessionComplete OnCreateSessionComplete;

    UPROPERTY(BlueprintAssignable, Category = "Sessions")
    FBLOnFindSessionsComplete OnFindSessionsComplete;

    UPROPERTY(BlueprintAssignable, Category = "Sessions")
    FBLOnJoinSessionComplete OnJoinSessionComplete;

    UPROPERTY(BlueprintAssignable, Category = "Sessions")
    FBLOnDestroySessionComplete OnDestroySessionComplete;

private:
    // ===== Online Subsystem internal callbacks =====

    void HandleCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void HandleFindSessionsComplete(bool bWasSuccessful);
    void HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
    void HandleDestroySessionComplete(FName SessionName, bool bWasSuccessful);

    /** Resolve and cache the session interface; returns nullptr if unavailable. */
    IOnlineSessionPtr GetSessionInterface() const;

    // ===== Delegate handles (must be removed after each op) =====

    FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
    FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
    FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
    FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;

    FDelegateHandle CreateSessionCompleteDelegateHandle;
    FDelegateHandle FindSessionsCompleteDelegateHandle;
    FDelegateHandle JoinSessionCompleteDelegateHandle;
    FDelegateHandle DestroySessionCompleteDelegateHandle;

    /** Search state for the in-flight FindSessions call. */
    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    /** Map to server-travel to once a hosted session is created. */
    FString PendingMatchMap;

    /** A session that must be destroyed before a new host attempt completes. */
    bool bCreateSessionOnDestroy = false;
    int32 PendingNumPublicConnections = 10;
    bool bPendingIsLAN = true;

    /** Settings key for the host-advertised lobby name. */
    static const FName ServerNameSettingsKey;
};
