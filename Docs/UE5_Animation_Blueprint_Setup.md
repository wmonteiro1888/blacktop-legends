# Blacktop Legends - UE5 Animation Blueprint Setup

## Overview

This document guides the setup of the Animation Blueprint (`ABP_BLCharacter`) in Unreal Engine 5 for the Blacktop Legends streetballer character.

## Animation Blueprint Asset Structure

### Parent Files
- **Skeleton**: `SK_Streetballer_Base` (61 bones)
- **Skeletal Mesh**: `SK_BLCharacter_Base`

### Animation Blueprint
- **Name**: `ABP_BLCharacter`
- **Target Skeleton**: `SK_Streetballer_Base`
- **Location**: `/Game/Blacktop/Art/Characters/Blueprints/`

## Animation Blueprint Architecture

### Event Graph

The Event Graph handles input and state variables:

```
Event Blueprint Update Animation
  ├─ Get Owning Pawn (BLCharacter)
  ├─ Get Movement Component
  ├─ Calculate Movement Speed (Velocity magnitude)
  ├─ Determine Movement State (Idle, Walk, Run, Sprint)
  ├─ Update HasBall flag (from BLCharacter possession)
  ├─ Update Direction (forward, left, right, backward from input)
  └─ Set all variables used in Anim Graph
```

### Key Variables

| Variable | Type | Purpose |
|----------|------|----------|
| `MovementSpeed` | Float | Current velocity magnitude |
| `MovementState` | Enum | Idle, Walk, Run, Sprint |
| `Direction` | Float | -180 to 180 degrees (facing direction) |
| `HasBall` | Bool | Is character holding ball? |
| `IsDribbling` | Bool | Is currently dribbling? |
| `IsAiming` | Bool | Aiming to shoot? |
| `IsInAir` | Bool | Is character airborne? |
| `PlayRate` | Float | Animation playback speed (0.8 to 1.2) |

---

## Anim Graph State Machine

### Root State Machine: `SM_RootMotion`

```
SM_RootMotion
├─ Idle
├─ Movement
├─ BallHandling
├─ Shooting
├─ Reaction
└─ Celebration
```

### State 1: Idle

**Entry Point**: When MovementSpeed < 10 and not HasBall

**Animation Pose**:
```
Select Idle animation based on context:
  If IsAiming → Idle_Neutral (ready for shot)
  If Defense → Idle_Defense
  Else → Idle_Breathing (default)
```

**Exit Transitions**:
- → Movement (when MovementSpeed > 15)
- → BallHandling (when HasBall)
- → Reaction (when Jump input)

---

### State 2: Movement

**Entry Point**: When MovementSpeed > 15

**Blend Space Logic**:

```
1. Determine Speed Tier:
   If MovementSpeed < 300 → Walk
   Else if MovementSpeed < 600 → Run
   Else → Sprint

2. Get Input Direction (X, Y from controller input)
   X = -1 (left) to 1 (right)
   Y = -1 (back) to 1 (forward)

3. Sample appropriate Blend Space:
   Use BS_Walk / BS_Run / BS_Sprint
   Input: (X, Y) from controller
   Output: Pose

4. Apply Root Motion:
   Enable root motion from animation
   Character moves with animation velocity
```

**Blend Space Configuration: BS_Walk**

```
Type: 2D Blend Space
X-Axis: Strafe (-1 Left to 1 Right)
Y-Axis: Direction (-1 Backward to 1 Forward)

Animations:
  (0, 1)     → Walk_Forward
  (1, 0)     → Walk_Right
  (-1, 0)    → Walk_Left
  (0, -1)    → Walk_Backward
  (0.7, 0.7)  → Diagonal blend of Forward + Right
  (etc. for all diagonals)
```

**Exit Transitions**:
- → Idle (when MovementSpeed < 10)
- → BallHandling (when HasBall)
- → Reaction (when Jump input)

---

### State 3: BallHandling

**Entry Point**: When HasBall == True

**Sub-State Machine: SM_BallHandling**

```
SM_BallHandling
├─ Dribbling
│  ├─ Idle Dribble
│  ├─ Walking Dribble
│  └─ Running Dribble
├─ Passing
│  ├─ Chest Pass
│  ├─ Bounce Pass
│  └─ Overhead Pass
└─ Ready (holding ball, not dribbling)
```

**Dribbling State**:
```
if MovementSpeed < 50:
  Play Dribble_Idle (looping)
else if MovementSpeed < 300:
  Play Dribble_Walk (looping)
else if MovementSpeed >= 300:
  Play Dribble_Run (looping)

Use Blend Space (BS_Dribble) to transition smoothly
between idle/walk/run dribble states
```

**Passing State**:
```
When Pass Input Pressed:
  Stop dribbling
  Play Pass_Chest (non-looping, 18 frames)
  Wait for animation to finish
  Return to Dribbling or Ready
```

**Exit Transitions**:
- → Shooting (when Shoot input + ball in hand)
- → Movement (when HasBall = False, lost ball)
- → Reaction (when Jump input)

---

### State 4: Shooting

**Entry Point**: When Shoot Input Pressed + HasBall

**Sub-State Machine: SM_Shooting**

```
SM_Shooting
├─ PreShoot (aim/prepare)
├─ ShootVariations
│  ├─ JumpShot_Mid
│  ├─ JumpShot_3Pt
│  ├─ Layup_Right
│  ├─ Layup_Left
│  └─ Dunk
└─ PostShoot (follow-through)
```

**Shooting Logic**:
```
1. Determine Shot Type based on distance to hoop:
   If Distance < 6ft → Layup
   If Distance 6-15ft → Mid-range
   If Distance > 20ft → 3-pointer

2. Play appropriate shot animation (non-looping)

3. At frame 50% of animation:
   Call GameMode::AttemptShot()
   Server checks if ball hit hoop
   Triggers score event or miss event

4. Animation completes
   Transition to Reaction or back to Idle
```

**Exit Transitions**:
- → Celebration (if shot made)
- → Reaction (if shot missed)
- → Movement (when animation finishes)

---

### State 5: Reaction

**Entry Point**: When triggered by gameplay events

**Sub-States**:
```
SM_Reaction
├─ Jump
├─ Land
├─ Foul
├─ BlockReaction
└─ Impact
```

**Jump State**:
```
When Jump Input or ball collision:
  Play Jump_Vertical (non-looping, 40 frames)
  Set IsInAir = True
```

**Land State**:
```
When character returns to ground:
  Play Land_Soft (non-looping, 12 frames)
  Set IsInAir = False
  Return to previous state (Movement or BallHandling)
```

**Exit Transitions**:
- → Idle or Movement (when animation finishes)
- → BallHandling (if ball caught mid-air)

---

### State 6: Celebration

**Entry Point**: When shot made or play happens

**Animation Selection**:
```
if ShotType == Dunk:
  Play Celebrate_Dunk
else if ShotType == 3Pointer:
  Play Celebrate_3Pointer
else:
  Play Celebrate_Bucket
```

**Exit Transitions**:
- → Idle (when animation finishes)

---

## Montage Setup (One-Shot Animations)

For animations that interrupt gameplay (shots, passes, reactions), use **Animation Montages**:

### Montage: Montage_Shoot

```
Composite Tracks:
  [Shoot_Jump_Mid] 0-40 frames
  [Shoot_Jump_3Pt] 0-45 frames
  [Shoot_Layup_Right] 0-30 frames
  [Shoot_Layup_Left] 0-30 frames
  [Shoot_Dunk] 0-35 frames

Notifies:
  Frame 20: Notify_BallRelease (call GameMode::AttemptShot)
  Frame 35: Notify_MontageEnd (return to idle)
```

### Montage: Montage_Pass

```
Composite Tracks:
  [Pass_Chest] 0-18 frames
  [Pass_Bounce] 0-18 frames
  [Pass_Overhead] 0-18 frames
  [Pass_Behind] 0-22 frames

Notifies:
  Frame 10: Notify_BallPass (call GameMode::MakePass)
  Frame 18: Notify_MontageEnd (return to dribbling)
```

---

## Blueprint Event Graph Code

### Update Animation Function

```cpp
Event Blueprint Update Animation
{
    // Get owning pawn (must be BLCharacter)
    ABlCharacter* OwnerCharacter = Cast<ABlCharacter>(TryGetPawnOwner());
    if (!OwnerCharacter) return;

    // Get current velocity
    FVector Velocity = OwnerCharacter->GetCharacterMovement()->Velocity;
    float Speed = Velocity.Length();
    
    // Update movement speed variable
    MovementSpeed = Speed;

    // Determine movement state
    if (Speed < 10.0f) {
        MovementState = EMovementState::Idle;
    } else if (Speed < 300.0f) {
        MovementState = EMovementState::Walk;
    } else if (Speed < 600.0f) {
        MovementState = EMovementState::Run;
    } else {
        MovementState = EMovementState::Sprint;
    }

    // Get input direction (from controller)
    FVector ForwardDirection = OwnerCharacter->GetActorForwardVector();
    FVector RightDirection = OwnerCharacter->GetActorRightVector();
    
    float InputX = OwnerCharacter->InputMovementX;
    float InputY = OwnerCharacter->InputMovementY;
    
    // Update direction for blend spaces
    Direction = FMath::Atan2(InputX, InputY) * (180.0f / PI);

    // Update ball possession
    HasBall = OwnerCharacter->IsBallHeld();
    IsDribbling = OwnerCharacter->IsDribbling();

    // Update animation play rate based on stamina
    float StaminaPercent = OwnerCharacter->GetStamina() / 100.0f;
    PlayRate = FMath::Clamp(1.0f - (1.0f - StaminaPercent) * 0.2f, 0.8f, 1.2f);
}
```

---

## Testing Checklist

- [ ] Idle animation loops smoothly
- [ ] Walk forward transitions smoothly from idle
- [ ] Run transitions smoothly from walk
- [ ] Blend space 2D directional movement works (diagonals)
- [ ] Sprint animation plays at correct speed
- [ ] Dribble animation loops while moving
- [ ] Jump animation plays on input
- [ ] Land animation plays on ground return
- [ ] Shoot animation plays and ball is released at correct frame
- [ ] Pass animation plays and ball is passed to teammate
- [ ] Celebration animation plays on made shot
- [ ] All transitions feel responsive (not sluggish)
- [ ] No animation popping/snapping
- [ ] Root motion movement matches character velocity

---

## Performance Notes

- **Animation Tick Rate**: 30 FPS (synced with UE5 default)
- **Blend Space Updates**: Per frame (low cost)
- **State Machine**: Optimized for real-time transitions
- **LOD**: Character visible from all camera angles (no LOD needed for park)

---

## Next Steps

1. ✅ Create Animation Blueprint in Persona editor
2. ⬜ Set up Event Graph with update logic
3. ⬜ Create state machine in Anim Graph
4. ⬜ Add blend spaces for movement
5. ⬜ Add montages for shooting/passing
6. ⬜ Test with BLCharacter in gameplay
7. ⬜ Tweak timing and transitions
