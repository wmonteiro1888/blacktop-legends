# Multiplayer Sessions — Host & Join (Desktop)

This document describes how a desktop client hosts and joins a 5v5 match in
Blacktop Legends. Session management lives in `UBLGameInstance`
(`Source/BlacktopLegends/Game/BLGameInstance.{h,cpp}`) and wraps Unreal's
Online Subsystem.

## What's included

| Class | Responsibility |
|-------|----------------|
| `UBLGameInstance` | Host / find / join / destroy sessions; map travel |
| `ABLGameMode` | Server-authoritative rules; balances players across Home/Away |
| `ABLPlayerState` | Replicated per-player team + score |

## Online backend

By default the project uses **`OnlineSubsystemNull`**, which supports **LAN**
host/find/join with no account or external service — ideal for testing two
desktops on the same network. Configuration lives in `Config/DefaultEngine.ini`.

To enable internet play, switch `DefaultPlatformService` to `Steam` (or another
provider) and add the matching `[OnlineSubsystemSteam]` block, then call the
session functions with `bIsLAN = false`.

## Flow

```
Host:   HostSession(NumPublicConnections=10, bIsLAN=true, MatchMap="HarlemCrown")
          -> CreateSession -> ServerTravel("/Game/Maps/HarlemCrown?listen")

Client: FindSessions(MaxSearchResults=20, bIsLAN=true)
          -> OnFindSessionsComplete(Results)        // array of FBLSessionInfo
        JoinFoundSession(Results[i].SearchResultIndex)
          -> JoinSession -> ClientTravel(resolved host address)
```

## Wiring the desktop menu (Blueprint)

`UBLGameInstance` exposes everything as `BlueprintCallable` functions and
`BlueprintAssignable` delegates, so a UMG main menu can drive it without extra
C++:

1. Get Game Instance → Cast to `BLGameInstance`.
2. **Host** button → `HostSession`.
3. **Find Matches** button → `FindSessions`; bind `OnFindSessionsComplete`
   to populate a server-list widget from the returned `FBLSessionInfo` array
   (`ServerName`, `PingMs`, `CurrentPlayers`/`MaxPlayers`).
4. **Join** on a row → `JoinFoundSession(SearchResultIndex)`; bind
   `OnJoinSessionComplete` to show success/failure.
5. **Leave** → `DestroySession`.

## Testing on one machine

Use the editor's **Play → Net Mode → Play As Client** with 2 players, or
launch two standalone instances and host on one, then `FindSessions` +
`JoinFoundSession` on the other (LAN works on `localhost`).

> Note: `MatchMap` ("HarlemCrown") and `/Game/Maps/...` paths assume those map
> assets exist. Adjust the names to match the project's content layout.
