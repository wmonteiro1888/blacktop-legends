# Blacktop Legends - Animation Library Plan

## Overview

This document outlines the complete animation library for the Blacktop Legends streetballer character. The library is designed for arcade-realistic basketball gameplay with 5v5 multiplayer and park-run authenticity.

## Animation Organization

Animations are grouped into functional categories, each with specific frame counts, loop markers, and blend space compatibility.

### Category Structure
1. **Idle & Stance** (8 animations)
2. **Movement** (16 animations)
3. **Ball Actions** (24 animations)
4. **Reactions** (12 animations)
5. **Celebrations** (8 animations)
6. **Transitions** (6 animations)

**Total Planned Animations**: 74 unique clips

---

## 1. Idle & Stance (8 animations)

| Name | Frames | Loop | Description |
|------|--------|------|-------------|
| `Idle_Neutral` | 120 | Yes | Relaxed stance, ready position |
| `Idle_Breathing` | 60 | Yes | Chest breathing, minimal movement |
| `Idle_Tired` | 90 | Yes | Fatigued stance, hands on knees |
| `Idle_Defense` | 100 | Yes | Defensive ready, crouched |
| `Stance_Wide` | 1 | No | Wide stance for jump/drive |
| `Stance_Narrow` | 1 | No | Narrow stance for passing |
| `Stance_Side` | 1 | No | Side-stepping ready position |
| `Stance_Back` | 1 | No | Backpedaling ready position |

---

## 2. Movement (16 animations)

### 2.1 Walking
| Name | Frames | Loop | Speed | Notes |
|------|--------|------|-------|-------|
| `Walk_Forward` | 40 | Yes | Slow | Forward walk, ball in hand |
| `Walk_Backward` | 40 | Yes | Slow | Backward walk |
| `Walk_Left` | 40 | Yes | Slow | Left strafe walk |
| `Walk_Right` | 40 | Yes | Slow | Right strafe walk |

### 2.2 Running
| Name | Frames | Loop | Speed | Notes |
|------|--------|------|-------|-------|
| `Run_Forward` | 30 | Yes | Medium | Normal running |
| `Run_Backward` | 30 | Yes | Medium | Running in reverse |
| `Run_Left` | 30 | Yes | Medium | Left strafe run |
| `Run_Right` | 30 | Yes | Medium | Right strafe run |

### 2.3 Sprinting
| Name | Frames | Loop | Speed | Notes |
|------|--------|------|-------|-------|
| `Sprint_Forward` | 24 | Yes | Fast | Fast attack/escape |
| `Sprint_Backward` | 24 | Yes | Fast | Fast backpedal |
| `Sprint_Left` | 24 | Yes | Fast | Left sprint strafe |
| `Sprint_Right` | 24 | Yes | Fast | Right sprint strafe |

### 2.4 Direction Blends
- Blend spaces for smooth transitions between forward/left/right/backward at each speed (Walk, Run, Sprint)
- Enable fluid movement without snapping between cardinal directions

---

## 3. Ball Actions (24 animations)

### 3.1 Pickup & Release
| Name | Frames | Loop | Description |
|------|--------|------|-------------|
| `Pickup_Ground_Front` | 20 | No | Pick up ball from ground (front) |
| `Pickup_Ground_Side` | 20 | No | Pick up ball from ground (side) |
| `Pickup_Pass_Catch` | 15 | No | Catch incoming pass |
| `Release_Idle` | 10 | No | Release ball to ground (standing) |
| `Release_Ground` | 8 | No | Drop ball quickly |

### 3.2 Dribbling
| Name | Frames | Loop | Description |
|------|--------|------|-------------|
| `Dribble_Idle` | 40 | Yes | Stationary dribble |
| `Dribble_Walk` | 40 | Yes | Dribble while walking |
| `Dribble_Run` | 30 | Yes | Dribble while running |
| `Dribble_Crossover` | 30 | No | Crossover dribble (ball between legs) |
| `Dribble_Behind` | 25 | No | Behind-the-back dribble |
| `Dribble_Hesitation` | 20 | No | Quick dribble hesitation |

### 3.3 Shooting
| Name | Frames | Loop | Description |
|------|--------|------|-------------|
| `Shoot_Jump_Mid` | 40 | No | Mid-range jump shot |
| `Shoot_Jump_3Pt` | 45 | No | 3-point jump shot |
| `Shoot_Layup_Right` | 30 | No | Layup (right hand) |
| `Shoot_Layup_Left` | 30 | No | Layup (left hand) |
| `Shoot_Dunk_Start` | 20 | No | Dunk approach (1 or 2-step) |
| `Shoot_Dunk_Finish` | 15 | No | Dunk finish |

### 3.4 Passing
| Name | Frames | Loop | Description |
|------|--------|------|-------------|
| `Pass_Chest` | 18 | No | Chest pass |
| `Pass_Bounce` | 18 | No | Bounce pass |
| `Pass_Overhead` | 18 | No | Overhead pass |
| `Pass_Behind` | 22 | No | Behind-the-back pass |

---

## 4. Reactions (12 animations)

| Name | Frames | Loop | Description |
|------|--------|------|-------------|
| `Jump_Vertical` | 40 | No | Vertical jump (reach) |
| `Jump_Forward` | 40 | No | Forward momentum jump |
| `Land_Soft` | 12 | No | Soft landing (normal) |
| `Land_Hard` | 15 | No | Hard landing (fatigue) |
| `Foul_Called` | 30 | No | Reaction to foul call |
| `Foul_Protest` | 45 | No | Protesting foul call |
| `Miss_Shot` | 30 | No | Reaction to missed shot |
| `Block_Reaction` | 25 | No | Reaction to block |
| `Steal_Reaction` | 20 | No | Reaction to steal |
| `Exhausted` | 60 | Yes | Heavy breathing, fatigue |
| `Pain_Reaction` | 30 | No | Minor injury reaction |
| `Fall_Recovery` | 35 | No | Get up from falling |

---

## 5. Celebrations (8 animations)

| Name | Frames | Loop | Description |
|------|--------|------|-------------|
| `Celebrate_Bucket` | 45 | No | Made shot celebration |
| `Celebrate_3Pointer` | 50 | No | 3-pointer celebration |
| `Celebrate_Dunk` | 55 | No | Dunk celebration |
| `Celebrate_Team_High_Five` | 30 | No | Team high-five |
| `Celebrate_Crowd_Pump` | 35 | No | Pump fist to crowd |
| `Celebrate_Showboat` | 60 | No | Extended showboat move |
| `Celebrate_Walk_Off` | 45 | No | Confident walk-off |
| `Chant_Hype` | 40 | Yes | Crowd hype chant |

---

## 6. Transitions (6 animations)

| Name | Frames | Loop | Description |
|------|--------|------|-------------|
| `Transition_RunToWalk` | 20 | No | Smooth run-to-walk deceleration |
| `Transition_WalkToRun` | 15 | No | Smooth walk-to-run acceleration |
| `Transition_RunToSprint` | 12 | No | Smooth run-to-sprint acceleration |
| `Transition_SprintToRun` | 15 | No | Smooth sprint-to-run deceleration |
| `Transition_DefenseStance` | 18 | No | Transition to defensive crouch |
| `Transition_ToShoot` | 12 | No | Transition to ready-to-shoot pose |

---

## Animation Naming Convention

### Format
```
[Action]_[Variation]_[Descriptor]
```

### Examples
- `Shoot_Jump_Mid` = Shooting action, jump variation, mid-range distance
- `Dribble_Crossover` = Dribbling action, crossover variation
- `Pass_Overhead` = Passing action, overhead variation
- `Idle_Defense` = Idle action, defensive variation

### Rules
- Use PascalCase (capital first letter of each word)
- No spaces, hyphens, or special characters
- Action = primary movement category
- Variation = style or direction modifier
- Descriptor = optional detail (distance, hand, direction)

---

## Blend Spaces

### 1. Movement Blend Spaces

#### BS_Walk (2D Blend Space)
- **X-Axis**: -1 (Left) to +1 (Right)
- **Y-Axis**: -1 (Backward) to +1 (Forward)
- **Animations**:
  - (0, 1) → Walk_Forward
  - (0, -1) → Walk_Backward
  - (-1, 0) → Walk_Left
  - (1, 0) → Walk_Right
  - (0.7, 0.7) → Walk_Forward + Left blend
  - Etc. for all diagonal combinations

#### BS_Run (2D Blend Space)
- Same structure as BS_Walk, using Run_* animations

#### BS_Sprint (2D Blend Space)
- Same structure as BS_Walk, using Sprint_* animations

### 2. Dribble Blend Space (1D)
- Input: Dribble_Type (0 = Idle, 0.5 = Walking, 1 = Running)
- Transitions between Dribble_Idle, Dribble_Walk, Dribble_Run

---

## Animation Blueprint State Machine

### States
1. **Idle** → Plays Idle_Neutral, transitions to Movement on input
2. **Movement** → Blend spaces handle directional movement
3. **BallHandling** → Dribble and pass states
4. **Shooting** → One-shot shoot animations
5. **Reaction** → Jump, land, foul reactions
6. **Celebration** → Made shot celebrations

### State Transitions
```
Idle ↔ Movement
  ↓
BallHandling → Shooting
  ↓
Reaction/Celebration → Back to Movement or Idle
```

---

## Frame Rate & Timing

- **Default Frame Rate**: 30 FPS (standard for UE5 animation)
- **Frame Duration**: 1 frame = 0.033 seconds
- **Example**: 30 frames = 1 second duration

### Calculation
```
Animation Duration (seconds) = Frame Count / 30

Example:
- 120 frames = 4 seconds (Idle_Neutral)
- 40 frames = 1.33 seconds (Shoot_Jump_Mid)
- 30 frames = 1 second (Run_Forward)
```

---

## Motion Capture / Animation Production Priority

### Phase 1: Core Gameplay (Week 1-2)
1. Idle_Neutral
2. Walk_Forward, Walk_Backward
3. Run_Forward, Run_Backward
4. Dribble_Idle, Dribble_Run
5. Shoot_Jump_Mid, Shoot_Layup_Right
6. Land_Soft

### Phase 2: Ball Interactions (Week 2-3)
7. Pickup_Ground_Front
8. Pass_Chest
9. Celebrate_Bucket
10. Catch pass animation

### Phase 3: Advanced Movement (Week 3)
11. Sprint_Forward
12. Dribble_Crossover
13. Shoot_Jump_3Pt, Shoot_Dunk_Start
14. Strafing animations (Walk_Left, Run_Left, etc.)

### Phase 4: Polish & Reactions (Week 4)
15. All Reaction animations
16. Celebration variations
17. Transition blend animations
18. Idle variations (Idle_Tired, Idle_Defense)

---

## Export Checklist

Before exporting animations from Maya:

- [ ] Each animation is in its own timeline/sequence
- [ ] Frame range matches specification (e.g., 120 frames for Idle_Neutral)
- [ ] Loop point is set correctly (if applicable)
- [ ] No keyframes outside the specified range
- [ ] Skeleton matches SK_Streetballer_Base (61 bones)
- [ ] Naming matches convention (PascalCase, no spaces)
- [ ] Test in preview to verify loop smoothness
- [ ] Export as individual FBX files or batch-export

---

## UE5 Import Settings

**Destination**: `/Game/Blacktop/Art/Characters/Animations/`

**Import Options**:
- ✅ Skeletal Mesh: NO (skeleton only)
- ✅ Animation: YES
- ✅ Skeleton: `SK_Streetballer_Base`
- ✅ Import Mesh: NO
- ✅ Create Physics Asset: NO

**Animation Naming**:
- Import as-is (preserve Maya naming)
- UE5 will add `_Anim` suffix automatically

---

## Next Steps

1. ✅ Create skeleton rig in Maya
2. ⬜ Bind character mesh to skeleton
3. ⬜ Create Phase 1 animations (6 core loops)
4. ⬜ Export to FBX
5. ⬜ Import into UE5
6. ⬜ Build Animation Blueprint with state machine
7. ⬜ Test in-game with BLCharacter controller
