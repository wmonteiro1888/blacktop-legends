#include "BLGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/PlayerController.h"

const FName UBLGameInstance::ServerNameSettingsKey = FName(TEXT("BL_SERVER_NAME"));

UBLGameInstance::UBLGameInstance()
{
    // Bind member functions to the engine's session-complete delegate signatures.
    // Handles are added/removed per operation in the Handle* / public methods.
    CreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UBLGameInstance::HandleCreateSessionComplete);
    FindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UBLGameInstance::HandleFindSessionsComplete);
    JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UBLGameInstance::HandleJoinSessionComplete);
    DestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UBLGameInstance::HandleDestroySessionComplete);
}

void UBLGameInstance::Init()
{
    Super::Init();

    if (IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
    {
        UE_LOG(LogTemp, Log, TEXT("[BL] Online subsystem: %s"), *Subsystem->GetSubsystemName().ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[BL] No online subsystem available; session features disabled."));
    }
}

IOnlineSessionPtr UBLGameInstance::GetSessionInterface() const
{
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    return Subsystem ? Subsystem->GetSessionInterface() : nullptr;
}

// ===== Host =====

void UBLGameInstance::HostSession(int32 NumPublicConnections, bool bIsLAN, const FString& MatchMap)
{
    IOnlineSessionPtr Sessions = GetSessionInterface();
    if (!Sessions.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("[BL] HostSession: no session interface."));
        OnCreateSessionComplete.Broadcast(false);
        return;
    }

    PendingMatchMap = MatchMap;
    PendingNumPublicConnections = NumPublicConnections;
    bPendingIsLAN = bIsLAN;

    // A stale session under the same name blocks creation. Destroy it first and
    // resume the host attempt from HandleDestroySessionComplete.
    if (Sessions->GetNamedSession(NAME_GameSession) != nullptr)
    {
        bCreateSessionOnDestroy = true;
        DestroySession();
        return;
    }

    FOnlineSessionSettings Settings;
    Settings.bIsLANMatch = bIsLAN;
    Settings.NumPublicConnections = FMath::Max(1, NumPublicConnections);
    Settings.NumPrivateConnections = 0;
    Settings.bShouldAdvertise = true;
    Settings.bAllowJoinInProgress = true;
    Settings.bAllowJoinViaPresence = true;
    Settings.bUsesPresence = true;
    Settings.bUseLobbiesIfAvailable = true;
    Settings.bAllowInvites = true;

    // Advertise a human-readable lobby name so the join menu can display it.
    FString HostName = TEXT("Blacktop Legends Match");
    if (const ULocalPlayer* LP = GetFirstGamePlayer())
    {
        const FString Nick = LP->GetNickname();
        if (!Nick.IsEmpty())
        {
            HostName = FString::Printf(TEXT("%s's Run"), *Nick);
        }
    }
    Settings.Set(ServerNameSettingsKey, HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    CreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

    const int32 ControllerId = GetFirstGamePlayer() ? GetFirstGamePlayer()->GetControllerId() : 0;
    if (!Sessions->CreateSession(ControllerId, NAME_GameSession, Settings))
    {
        Sessions->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
        UE_LOG(LogTemp, Warning, TEXT("[BL] CreateSession call failed to start."));
        OnCreateSessionComplete.Broadcast(false);
    }
}

void UBLGameInstance::HandleCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (IOnlineSessionPtr Sessions = GetSessionInterface())
    {
        Sessions->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
    }

    OnCreateSessionComplete.Broadcast(bWasSuccessful);

    if (bWasSuccessful)
    {
        UWorld* World = GetWorld();
        if (World && !PendingMatchMap.IsEmpty())
        {
            // Open the match map as a listen server so clients can connect.
            const FString TravelURL = FString::Printf(TEXT("/Game/Maps/%s?listen"), *PendingMatchMap);
            World->ServerTravel(TravelURL);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[BL] Failed to create session '%s'."), *SessionName.ToString());
    }
}

// ===== Find =====

void UBLGameInstance::FindSessions(int32 MaxSearchResults, bool bIsLAN)
{
    IOnlineSessionPtr Sessions = GetSessionInterface();
    if (!Sessions.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("[BL] FindSessions: no session interface."));
        OnFindSessionsComplete.Broadcast(TArray<FBLSessionInfo>(), false);
        return;
    }

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->bIsLanQuery = bIsLAN;
    SessionSearch->MaxSearchResults = FMath::Max(1, MaxSearchResults);
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    FindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

    const int32 ControllerId = GetFirstGamePlayer() ? GetFirstGamePlayer()->GetControllerId() : 0;
    if (!Sessions->FindSessions(ControllerId, SessionSearch.ToSharedRef()))
    {
        Sessions->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
        UE_LOG(LogTemp, Warning, TEXT("[BL] FindSessions call failed to start."));
        OnFindSessionsComplete.Broadcast(TArray<FBLSessionInfo>(), false);
    }
}

void UBLGameInstance::HandleFindSessionsComplete(bool bWasSuccessful)
{
    if (IOnlineSessionPtr Sessions = GetSessionInterface())
    {
        Sessions->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
    }

    TArray<FBLSessionInfo> Results;
    if (bWasSuccessful && SessionSearch.IsValid())
    {
        for (int32 Index = 0; Index < SessionSearch->SearchResults.Num(); ++Index)
        {
            const FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[Index];
            if (!Result.IsValid())
            {
                continue;
            }

            FBLSessionInfo Info;
            Info.SearchResultIndex = Index;
            Info.PingMs = Result.PingInMs;

            const FOnlineSessionSettings& S = Result.Session.SessionSettings;
            Info.MaxPlayers = S.NumPublicConnections;
            Info.CurrentPlayers = S.NumPublicConnections - Result.Session.NumOpenPublicConnections;

            FString ServerName;
            if (!S.Get(ServerNameSettingsKey, ServerName) || ServerName.IsEmpty())
            {
                ServerName = Result.Session.OwningUserName;
            }
            Info.ServerName = ServerName;

            Results.Add(Info);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[BL] FindSessions complete: %d result(s), success=%d"), Results.Num(), bWasSuccessful);
    OnFindSessionsComplete.Broadcast(Results, bWasSuccessful);
}

// ===== Join =====

void UBLGameInstance::JoinFoundSession(int32 SearchResultIndex)
{
    IOnlineSessionPtr Sessions = GetSessionInterface();
    if (!Sessions.IsValid() || !SessionSearch.IsValid() ||
        !SessionSearch->SearchResults.IsValidIndex(SearchResultIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("[BL] JoinFoundSession: invalid index %d or no search state."), SearchResultIndex);
        OnJoinSessionComplete.Broadcast(false);
        return;
    }

    JoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

    const int32 ControllerId = GetFirstGamePlayer() ? GetFirstGamePlayer()->GetControllerId() : 0;
    if (!Sessions->JoinSession(ControllerId, NAME_GameSession, SessionSearch->SearchResults[SearchResultIndex]))
    {
        Sessions->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
        UE_LOG(LogTemp, Warning, TEXT("[BL] JoinSession call failed to start."));
        OnJoinSessionComplete.Broadcast(false);
    }
}

void UBLGameInstance::HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    IOnlineSessionPtr Sessions = GetSessionInterface();
    if (Sessions.IsValid())
    {
        Sessions->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
    }

    const bool bWasSuccessful = (Result == EOnJoinSessionCompleteResult::Success);
    OnJoinSessionComplete.Broadcast(bWasSuccessful);

    if (!bWasSuccessful || !Sessions.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("[BL] JoinSession '%s' failed (result %d)."), *SessionName.ToString(), (int32)Result);
        return;
    }

    // Resolve the host's connect string and travel the local client to it.
    FString ConnectString;
    if (!Sessions->GetResolvedConnectString(NAME_GameSession, ConnectString) || ConnectString.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[BL] Could not resolve connect string for '%s'."), *SessionName.ToString());
        return;
    }

    if (APlayerController* PC = GetFirstLocalPlayerController())
    {
        PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
    }
}

// ===== Destroy =====

void UBLGameInstance::DestroySession()
{
    IOnlineSessionPtr Sessions = GetSessionInterface();
    if (!Sessions.IsValid())
    {
        OnDestroySessionComplete.Broadcast(false);
        return;
    }

    DestroySessionCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);
    if (!Sessions->DestroySession(NAME_GameSession))
    {
        Sessions->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
        OnDestroySessionComplete.Broadcast(false);
    }
}

void UBLGameInstance::HandleDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (IOnlineSessionPtr Sessions = GetSessionInterface())
    {
        Sessions->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
    }

    OnDestroySessionComplete.Broadcast(bWasSuccessful);

    // If this destroy was a precursor to (re)hosting, fire the host attempt now.
    if (bCreateSessionOnDestroy && bWasSuccessful)
    {
        bCreateSessionOnDestroy = false;
        HostSession(PendingNumPublicConnections, bPendingIsLAN, PendingMatchMap);
    }
}
