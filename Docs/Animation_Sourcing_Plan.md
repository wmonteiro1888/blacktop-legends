# Blacktop Legends - Animation Sourcing Plan

## Strategy

Instead of hand-animating all 74 clips from `Animation_Library_Plan.md` in Maya, we source
professional basketball mocap and retarget it onto `SK_Streetballer_Base` (61 bones) using
UE5's IK Retargeter. Custom Maya animation is reserved for signature streetball flair only.

**Why this works for basketball (and wouldn't for soccer):** the ball lives in the hands,
not at the feet. Mocap packs never include the ball — and they don't need to. The ball is a
separate physics actor attached to a hand socket while held (our skeleton already has
`Index_L_Socket` for this), detached on shot/pass release, and driven procedurally during
dribbles via anim-notify contact frames. Every humanoid mocap clip "just works."

### Three phases

1. **Prove the pipeline (free, ~1 evening)** — retarget 5-6 free clips onto our skeleton,
   get one character dribbling in-engine.
2. **Buy coverage** — the 167-clip basketball mocap pack on Fab covers most of the 74-clip
   plan in one purchase.
3. **Custom gaps only** — Harlem Crown-specific flair (showboat celebrations, foul protest,
   signature moves) in Maya. Expect ~10 clips, not 74.

---

## Sources

| Source | What | Cost | Link |
|--------|------|------|------|
| Fab: Basketball mocap pack | 167 basketball mocap clips, humanoid FBX, UE-compatible, 5.0 rating | Paid (check in Epic account) | https://www.fab.com/listings/b13b7b06-8c0f-435e-88df-0b616c15e468 |
| Fab: Basketball Emotes (Araz Creates) | Celebration/emote mocap collection | Paid | Search "Basketball Emotes" on Fab |
| Mixamo | Free basketball clips: idle dribble, jump shot, layup + full locomotion library | Free | https://www.mixamo.com |
| Rokoko free sports pack | 12 free sports mocap animations | Free | https://www.rokoko.com/resources/rokoko-mocap-12-free-sports-animations |
| MoCap Online | Sports library incl. basketball; free sampler to test pipeline | Free sampler / paid packs $30+ | https://mocaponline.com |
| ActorCore (Reallusion) | Basketball motions, online retargeting to uploaded characters | Per-motion | https://actorcore.reallusion.com/3d-motion |

---

## Coverage mapping (74-clip plan vs. sources)

| Category (count) | Expected coverage | Source |
|------------------|-------------------|--------|
| Idle & Stance (8) | Full | Any pack + Mixamo |
| Movement (16) | Full | Fab pack / Mixamo locomotion |
| Ball Actions (24) | Near-full — dribbles, shots, layups, dunks, passes are standard mocap fare | Fab basketball pack |
| Reactions (12) | Partial — jumps/lands yes; foul protest likely custom | Fab pack + custom |
| Celebrations (8) | Full | Basketball Emotes pack |
| Transitions (6) | Optional — UE5 blend/inertialization may make dedicated clips unnecessary | Engine-side first |

---

## Retargeting pipeline (one-time setup)

### 1. IK Rig for our skeleton
Create `IK_Streetballer` on `SK_Streetballer_Base`. Retarget chains (bone names per
`README_Character_Skeleton_Reference.md`):

| Chain | Start → End |
|-------|-------------|
| Root | root |
| Pelvis (retarget root) | Pelvis |
| Spine | Spine_01 → Spine_03 |
| Neck | Neck_01 |
| Head | Head |
| LeftArm | Shoulder_L → Hand_L (RightArm mirrored) |
| LeftLeg | Thigh_L → Ball_L (RightLeg mirrored) |
| Fingers | Per-finger chains (Thumb/Index/Middle/Ring/Pinky _01 → _02) |

### 2. IK Rig for the source skeleton
- UE5 Manny sources: engine ships `IK_Mannequin` — use as-is.
- Mixamo sources: import one Mixamo FBX, build an IK Rig once (or use T-pose import; our
  skeleton is T-pose, Manny is A-pose — set the retarget pose accordingly).

### 3. IK Retargeter
Create `RTG_Source_to_Streetballer` (source rig → `IK_Streetballer`), map chains, fix the
retarget pose (A-pose vs T-pose), then batch-select animations in the Asset Browser and
**Export Selected Animations** to `/Game/Blacktop/Art/Characters/Animations/`.

### 4. Rename to convention
Batch-rename exported clips to match `Animation_Library_Plan.md` names
(`Shoot_Jump_Mid`, `Dribble_Crossover`, etc.) so the anim blueprint doc stays accurate.

### 5. Ball integration
- Held: attach ball actor to `Index_L_Socket` / right-hand equivalent.
- Dribble: add anim notifies at hand-contact frames on each dribble clip; gameplay code
  interpolates the ball between hand position and floor bounce between notifies.
- Release: detach on shot/pass notify, hand off to physics with computed impulse.

---

## First session checklist (on the dev machine)

1. Pull this branch.
2. Download the free Mixamo basketball clips (idle dribble, jump shot, layup) +
   Rokoko free sports pack.
3. Import one clip into UE5, build `IK_Streetballer` per the chain table above.
4. Retarget the idle dribble onto `SK_Streetballer_Base` — this is the go/no-go test.
5. If clean: buy the Fab basketball pack and batch-retarget.
6. If joints look wrong: fix chain mapping / retarget pose before buying anything.
