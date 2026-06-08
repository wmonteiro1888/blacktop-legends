# Blacktop Legends - Complete Character Implementation Guide

## Overview

This guide walks through the complete workflow for implementing the Blacktop Legends streetballer character from Maya to in-game gameplay.

## Phase 1: Maya Skeleton & Rigging (Week 1)

### Step 1.1: Create Skeleton in Maya

1. Open Maya
2. Set workspace to Centimeters:
   ```
   Edit → Preferences → Settings
   Working Units: cm
   ```

3. Use the MEL script from `README_Character_Rigging_Workflow.md`:
   - Copy joint creation commands
   - Paste into Script Editor
   - Execute to create 61-bone skeleton

4. Verify skeleton structure in Outliner:
   ```
   root
   ├── Pelvis
   ├── Spine_01 → Spine_02 → Spine_03 → Neck_01 → Head
   ├── Thigh_L → ... → Toe_L_01
   ├── Thigh_R → ... → Toe_R_01
   ├── Shoulder_L → ... → Hand_L → Fingers
   └── Shoulder_R → ... → Hand_R → Fingers
   ```

### Step 1.2: Orient Joints

```mel
// Select root joint
Skeleton → Orient Joints → Orient Joints Options
  Orient: XYZ
  Second Axis World Up: Y
  Apply

// Set preferred angle (bind pose)
Skeleton → Set Preferred Angle

// Freeze transforms
Modify → Freeze Transformations
```

### Step 1.3: Import & Bind Character Mesh

1. File → Import → Select character mesh FBX
2. Select mesh + root joint (Shift+click)
3. Skin → Bind Skin → Bind Skin Options
   - Bind To: Hierarchy
   - Max Influences: 4
   - Falloff: Smooth Bind
   - Apply

4. Paint weights:
   ```
   Skin → Paint Skin Weights
   - Paint arm vertices to Arm_L/Forearm_L
   - Paint leg vertices to Thigh_L/Calf_L
   - Paint torso smoothly across spine
   - Test rotations (rotate joint, check deformation)
   ```

### Step 1.4: Export Skeleton

```mel
File → Export Selection
Filename: SM_Streetballer_Base_Skeleton.fbx
Location: SourceAssets/Exports/FBX/Characters/
Format: FBX 2020
Options:
  ✓ Smooth Skin
  ✓ Blend Shapes
  ✗ Animation (skeleton only)
  ✗ Constraints
```

---

## Phase 2: Animation Creation (Week 2-3)

### Step 2.1: Create First Animation Loop (Idle_Neutral)

1. In Maya, select the skeleton root
2. Animation menu set → Key → Set Driven Key
3. Frame 0-120: Neutral breathing pose
   - Slight spine bend variation
   - Head natural tilt
   - Arms at rest
4. Add loop: Timeline → Right-click → Cycle with Offset

### Step 2.2: Create Movement Animations (Phase 1)

Priority order:
1. `Walk_Forward` (40 frames, loop)
2. `Run_Forward` (30 frames, loop)
3. `Sprint_Forward` (24 frames, loop)
4. `Dribble_Idle` (40 frames, loop)
5. `Shoot_Jump_Mid` (40 frames, no loop)
6. `Land_Soft` (12 frames, no loop)

### Step 2.3: Batch Export Animations

**Option A: Using Python Script**

```
1. Script → Open Script Editor
2. Python tab → Load export_animations_batch.py
3. Run script
4. Select export directory
5. All 74 animations export automatically to:
   SourceAssets/Exports/FBX/Characters/[Category]/
```

**Option B: Manual Export**

```mel
// For each animation:
File → Export Selection
Set frame range: start_frame to end_frame
Options: Same as Step 1.4
```

---

## Phase 3: UE5 Skeleton Import

### Step 3.1: Create Folder Structure

In UE5 Content Browser:
```
/Game/Blacktop/Art/Characters/
├── Skeletons/
│   └── SK_Streetballer_Base (directory)
├── Animations/
│   ├── Idle/ (directory)
│   ├── Movement/ (directory)
│   ├── BallActions/ (directory)
│   ├── Reactions/ (directory)
│   ├── Celebrations/ (directory)
│   └── Transitions/ (directory)
└── Blueprints/
    ├── ABP_BLCharacter (Animation Blueprint)
    ├── BP_BLCharacter (Character Blueprint)
    └── BP_BLCharacterController (Controller Blueprint)
```

### Step 3.2: Import Skeleton

1. Right-click in Content Browser → Import
2. Select: `SM_Streetballer_Base_Skeleton.fbx`
3. Import Options:
   - Skeletal Mesh: ✓ (Create New)
   - Skeleton: ✓ (Create New → SK_Streetballer_Base)
   - Materials: ✓ (Create New)
   - Import Mesh: ✓
   - Import Materials: ✓
   - Import Textures: ✓
4. Click Import

### Step 3.3: Verify Skeleton

1. Open `SK_Streetballer_Base` asset
2. Skeleton Tree: Verify 61 bones
3. Rotate bones to test skin deformation
4. Ensure physics asset was created (ragdoll)

---

## Phase 4: Animation Import (Batch)

### Step 4.1: Organize FBX Files

File system should be:
```
SourceAssets/Exports/FBX/Characters/
├── Idle/
│   ├── SM_Idle_Neutral.fbx
│   ├── SM_Idle_Breathing.fbx
│   ├── SM_Idle_Tired.fbx
│   └── SM_Idle_Defense.fbx
├── Movement/
│   ├── SM_Walk_Forward.fbx
│   ├── SM_Run_Forward.fbx
│   └── ... (12 more movement anims)
├── BallActions/
├── Reactions/
├── Celebrations/
└── Transitions/
```

### Step 4.2: Batch Import Animations (Python)

**In UE5 Editor:**

1. Window → Developer Tools → Python Console
2. Copy script from `Tools/Python/import_animations_ue5.py`
3. Paste into Python Console and Execute
4. All animations import automatically with correct skeleton reference

**Manual Import (if script fails):**

1. Right-click → Import
2. Select FBX animation file
3. Import Options:
   - Skeletal Mesh: ✗ (Animation only)
   - Animation: ✓
   - Skeleton: Select `SK_Streetballer_Base`
   - Destination Path: `/Game/Blacktop/Art/Characters/Animations/[Category]/`
4. Click Import

---

## Phase 5: Animation Blueprint Setup

### Step 5.1: Create Animation Blueprint

1. Right-click in Blueprints folder → Create Blueprint
2. Class: Animation Blueprint
3. Target Skeleton: `SK_Streetballer_Base`
4. Name: `ABP_BLCharacter`

### Step 5.2: Build State Machine (from guide)

**Event Graph:**
- Implement `NativeUpdateAnimation()` override
- Copy code from `BLCharacterAnimInstance.cpp`
- Update movement speed, state, direction, ball possession

**Anim Graph:**

1. Create State Machine: `SM_RootMotion`

2. Add States:
   - Idle
   - Movement
   - BallHandling
   - Shooting
   - Reaction
   - Celebration

3. State: Idle
   ```
   Animation: Idle_Neutral
   Transitions:
   - To Movement (MovementSpeed > 15)
   - To BallHandling (bHasBall)
   ```

4. State: Movement
   ```
   Blend Space 2D:
   - BS_Walk (X: Direction -1 to 1, Y: Forward -1 to 1)
   - BS_Run
   - BS_Sprint
   Use MovementState to select blend space
   ```

5. Complete remaining states following `UE5_Animation_Blueprint_Setup.md`

### Step 5.3: Create Blend Spaces

**BS_Walk (2D Blend Space):**
```
X-Axis: Strafe (-1 Left to 1 Right)
Y-Axis: Forward (-1 Back to 1 Forward)

Animation Points:
(0, 1) → Walk_Forward
(1, 0) → Walk_Right
(-1, 0) → Walk_Left
(0, -1) → Walk_Backward
(0.7, 0.7) → (blended)
(etc. for all quadrants)
```

Repeat for `BS_Run` and `BS_Sprint` with their respective animations.

---

## Phase 6: Character & Controller Setup

### Step 6.1: Update BLCharacter Class

1. Add to `BLCharacter.h`:
   ```cpp
   // Animation montages
   UPROPERTY(EditAnywhere, Category = "Animation")
   UAnimMontage* ShootMontage;
   
   UPROPERTY(EditAnywhere, Category = "Animation")
   UAnimMontage* PassMontage;
   
   // Animation instance
   UPROPERTY(BlueprintReadOnly, Category = "Animation")
   UBLCharacterAnimInstance* AnimInstance;
   
   // Getters
   bool IsBallHeld() const { return bHasBall; }
   bool IsDribbling() const { return bIsDribbling; }
   bool IsAiming() const { return bIsAiming; }
   float GetStamina() const { return CurrentStamina; }
   float GetMaxStamina() const { return MaxStamina; }
   
   // Setters
   void RequestShoot();
   void CancelShoot();
   void RequestPass();
   void ToggleDribble();
   void StartSprinting();
   void StopSprinting();
   ```

2. Implement in `BLCharacter.cpp`:
   ```cpp
   void ABlCharacter::BeginPlay()
   {
       Super::BeginPlay();
       AnimInstance = Cast<UBLCharacterAnimInstance>(GetMesh()->GetAnimInstance());
   }
   
   void ABlCharacter::RequestShoot()
   {
       if (bHasBall && !bIsShootingMontageActive)
       {
           PlayAnimMontage(ShootMontage);
           bIsShootingMontageActive = true;
       }
   }
   // ... implement others
   ```

### Step 6.2: Create Character Blueprint

1. Right-click → Create Blueprint Class
2. Parent: `ABlCharacter`
3. Name: `BP_BLCharacter`
4. Set Properties:
   - Skeletal Mesh: `SK_Streetballer_Base`
   - Animation Blueprint: `ABP_BLCharacter`
   - Shoot Montage: `Montage_Shoot`
   - Pass Montage: `Montage_Pass`
5. Compile & Save

### Step 6.3: Create Controller Blueprint

1. Right-click → Create Blueprint Class
2. Parent: `ABLCharacterController`
3. Name: `BP_BLCharacterController`
4. Set Input Assets:
   - Input Mapping Context: `IMC_Default` (or create new)
   - Move Action: `IA_Move`
   - Look Action: `IA_Look`
   - Jump Action: `IA_Jump`
   - Shoot Action: `IA_Shoot`
   - Pass Action: `IA_Pass`
   - Sprint Action: `IA_Sprint`
   - Dribble Action: `IA_Dribble`
5. Compile & Save

---

## Phase 7: Testing in Game

### Step 7.1: Setup Test Level

1. Create new level: `L_CharacterTest`
2. Drag `BP_BLCharacter` into level
3. Set as possessed by `BP_BLCharacterController`
4. Ensure ball spawn point exists

### Step 7.2: Test Checklist

- [ ] Character spawns in correct pose
- [ ] WASD movement works (walk animations)
- [ ] Sprint key increases speed (sprint animations)
- [ ] Jump works and plays jump animation
- [ ] Land animation plays on ground return
- [ ] Pickup ball from ground
- [ ] Dribble while moving
- [ ] Shoot animation plays (press LMB)
- [ ] Pass animation plays (press E)
- [ ] All blend space transitions smooth
- [ ] Stamina affects animation speed
- [ ] Celebration plays on made shot

### Step 7.3: Debug Checklist

- [ ] Animation Blueprint Event Graph updating correctly
- [ ] Movement speed values look correct
- [ ] Blend space inputs (-1 to 1) correct
- [ ] No animation popping/snapping
- [ ] Root motion movement matches character velocity
- [ ] Montages complete without getting stuck

---

## Phase 8: Production Workflow

### Creating New Animations

1. **In Maya:**
   ```
   - Open character scene
   - Create new animation at end of timeline
   - Key frames 1-40 (or appropriate range)
   - Export as SM_[AnimName].fbx
   ```

2. **In UE5:**
   ```
   - Import FBX to correct category folder
   - Reference SK_Streetballer_Base skeleton
   - Test in ABP_BLCharacter (if new state)
   - Add to state machine if new gameplay action
   ```

### Quick Iteration Loop

```
Maya Changes → Export FBX → Reimport in UE5 → Test → Repeat
```

---

## Troubleshooting

### Animation not playing
- Check: Animation Blueprint is assigned to skeletal mesh
- Check: State machine can reach that state
- Check: Animation is imported to correct skeleton

### Animation looks wrong
- Re-examine weights in Maya
- Reimport skeleton and rebind skin
- Check bone rotation values

### Blending not smooth
- Verify blend space setup (points properly distributed)
- Check animation frame counts (shouldn't be too short)
- Ensure loop animations have seamless loop

### Montage gets stuck
- Add notify at end of montage to return to state machine
- Verify montage animation references correct skeleton
- Check animation duration is correct

---

## Performance Notes

- 74 animations @ 30 FPS = ~240 MB disk space
- Skeleton bones (61) + animations in memory ~5-8 MB per character
- Blend spaces update per frame (minimal cost)
- State machine transitions ~1ms per frame
- Animation playback syncs with 30 FPS game tick

---

## Next Steps

✅ Phase 1: Skeleton & Rigging (Done)
✅ Phase 2: Animation Creation (Started)
⬜ Phase 3: Test & Iterate Animations
⬜ Phase 4: Gameplay Integration (Ball possession, shooting, scoring)
⬜ Phase 5: Multiplayer Replication (Network animation states)
⬜ Phase 6: Polish & Performance (LOD, compression, optimization)

---

**Questions?** Reference:
- `README_Character_Skeleton_Reference.md` - Joint hierarchy
- `README_Character_Rigging_Workflow.md` - Maya setup
- `Animation_Library_Plan.md` - Animation specifications
- `UE5_Animation_Blueprint_Setup.md` - Blueprint architecture
