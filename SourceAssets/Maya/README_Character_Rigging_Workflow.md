# Blacktop Legends - Maya Character Rigging Workflow

## Step 1: Create Base Skeleton (T-Pose)

### 1.1 New Scene Setup
```
File → New Scene
Edit → Preferences → Settings
  └─ Working Units: Centimeters
  └─ Linear: cm
  └─ Angular: Degrees
```

### 1.2 Create Root Joint
```mel
// In Maya Script Editor
joint -n "root" -p 0 0 0;
```

### 1.3 Pelvis (Hip Center)
```mel
joint -n "Pelvis" -p 0 10 0;
```

Position: **Y = 10 cm** above ground (base of spine)

### 1.4 Spine Chain
```mel
joint -n "Spine_01" -p 0 10 0;  // Lower torso
joint -n "Spine_02" -p 0 15 0;  // Mid torso
joint -n "Spine_03" -p 0 15 0;  // Upper chest
joint -n "Neck_01"  -p 0 8 0;   // Neck
joint -n "Head"     -p 0 10 0;  // Head top
// Total spine height: 58 cm from pelvis
```

### 1.5 Create Left Leg (FK Forward Kinematics)

**Parent chain**: Pelvis → Thigh_L → Calf_L → Foot_L → Ball_L → Toe_L_01

```mel
// Select Pelvis, then create chain
joint -n "Thigh_L"      -p -8 -10 0;    // Hip socket
joint -n "ThighTwist_L_01" -p 0 -15 0; // Twist relief
joint -n "Calf_L"       -p 0 -35 0;    // Knee
joint -n "CalfTwist_L_01" -p 0 -15 0;  // Twist relief
joint -n "Foot_L"       -p 0 -35 0;    // Ankle
joint -n "Ball_L"       -p 0 -8 3;     // Ball of foot
joint -n "Toe_L_01"     -p 0 -3 0;     // Toe
```

**Left leg total length**: ~106 cm

### 1.6 Create Right Leg (Mirror)

```mel
// Duplicate and mirror the left leg
// Select Thigh_L through Toe_L_01
// Edit → Duplicate Special
  // ✅ Duplicate Input Graph
  // ✅ Instance
  // ✅ Translate: X = 16 (to mirror across X)
// Rename all _L suffixes to _R
```

**OR manually create**:
```mel
// Select Pelvis
joint -n "Thigh_R"      -p 8 -10 0;    // Mirror of Thigh_L
joint -n "ThighTwist_R_01" -p 0 -15 0;
joint -n "Calf_R"       -p 0 -35 0;
joint -n "CalfTwist_R_01" -p 0 -15 0;
joint -n "Foot_R"       -p 0 -35 0;
joint -n "Ball_R"       -p 0 -8 3;
joint -n "Toe_R_01"     -p 0 -3 0;
```

### 1.7 Create Left Arm (FK)

**Parent chain**: Spine_03 → Shoulder_L → Arm_L → Forearm_L → Hand_L

```mel
// Select Spine_03
joint -n "Shoulder_L"   -p -18 0 0;     // Shoulder socket
joint -n "Arm_L"        -p -15 0 0;     // Upper arm
joint -n "ArmTwist_L_01" -p 0 -8 0;    // Twist relief 1/3
joint -n "ArmTwist_L_02" -p 0 -8 0;    // Twist relief 2/3
joint -n "Forearm_L"    -p 0 -18 0;    // Elbow-to-wrist
joint -n "ForearmTwist_L_01" -p 0 -6 0;// Twist relief 1/3
joint -n "ForearmTwist_L_02" -p 0 -6 0;// Twist relief 2/3
joint -n "Hand_L"       -p 0 -12 0;    // Wrist/hand base
```

**Left arm total length**: ~54 cm

### 1.8 Create Left Fingers (8 finger bones + 1 socket)

```mel
// Select Hand_L, then create finger chains

// THUMB (2 bones)
joint -n "Thumb_L_01"   -p 2 0 3;      // Base knuckle
joint -n "Thumb_L_02"   -p 1 0 2;      // Tip

// INDEX (2 bones)
// Select Hand_L again
joint -n "Index_L_01"   -p 3 0 -1;
joint -n "Index_L_02"   -p 1 0 -1;

// MIDDLE (2 bones)
// Select Hand_L again
joint -n "Middle_L_01"  -p 2 0 -2;
joint -n "Middle_L_02"  -p 1 0 -2;

// RING (2 bones)
// Select Hand_L again
joint -n "Ring_L_01"    -p 1 0 -3;
joint -n "Ring_L_02"    -p 1 0 -3;

// PINKY (2 bones)
// Select Hand_L again
joint -n "Pinky_L_01"   -p 0 0 -4;
joint -n "Pinky_L_02"   -p 0 0 -4;

// HAND SOCKET (for ball handling without fingers)
// Select Hand_L
joint -n "Index_L_Socket" -p 3 0 -1;   // Neutral hand position
```

**Total finger bones**: 10 + 1 socket = 11 bones

### 1.9 Create Right Arm & Fingers (Mirror)

```mel
// Mirror entire left arm+fingers to right side
// Duplicate left arm with X translation = 36 (shoulder width)
// OR manually create with _R suffixes at positive X positions

// Shoulder_R at +18 (mirror of -18)
// Arm_R at +15 (mirror of -15)
// etc.
```

### 1.10 Verify Skeleton Structure

```mel
// Check hierarchy
outliner -e -showDagObjects true (opens Outliner)

// Visual hierarchy should look like:
// root
//   ├── Pelvis
//   │   ├── Spine_01
//   │   │   ├── Spine_02
//   │   │   │   ├── Spine_03
//   │   │   │   │   ├── Neck_01
//   │   │   │   │   │   └── Head
//   │   │   │   │   ├── Shoulder_L
//   │   │   │   │   │   └── Arm_L → ... → Hand_L → Fingers
//   │   │   │   │   └── Shoulder_R (mirror)
//   │   ├── Thigh_L → ... → Toe_L_01
//   │   └── Thigh_R → ... → Toe_R_01
```

## Step 2: Orient Joints & Set Preferred Angle

### 2.1 Joint Orient Every Joint Properly

```mel
// Select root, then skeleton
Skeleton → Orient Joints → Orient Joints Options
  ├─ Orient: XYZ
  ├─ Second Axis World Up: Y
  ├─ Flip All
  └─ Apply
```

### 2.2 Set Preferred Angle (Bind Pose)

```mel
// Select root
// Skeleton → Set Preferred Angle
// This freezes the current pose as the bind pose for skinning
```

### 2.3 Freeze Transforms

```mel
// Select root
// Modify → Freeze Transformations
// This zeros out all transforms (rotate/translate to 0)
// Delete History (Edit → Delete All by Type → History)
```

## Step 3: Create IK Chains (Optional but Recommended)

### 3.1 Leg IK (for foot placement in animation)

```mel
// Create IK Handle from Thigh_L to Foot_L
Skeleton → Create IK Handle

// Select Thigh_L (start)
// Select Foot_L (end)
// Solver: ikRPsolver (Rotate Plane Solver)
// Name: IK_Foot_L

// Repeat for right leg: IK_Foot_R
```

### 3.2 Arm IK (for hand reach)

```mel
// Create IK Handle from Arm_L to Hand_L
// Solver: ikRPsolver
// Name: IK_Hand_L

// Repeat for right arm: IK_Hand_R
```

### 3.3 Pole Vectors (for IK bending direction)

```mel
// Create locators for pole vector targets
Create → Locator
  ├─ Name: Pole_Knee_L
  ├─ Position: in front of knee (Z + 10)
  
Create → Locator
  ├─ Name: Pole_Elbow_L
  ├─ Position: in front of elbow (Z + 8)

// Parent pole vectors to skeleton for organization
// (Optional for gameplay; animators may prefer FK only)
```

## Step 4: Import Character Mesh & Bind Skin

### 4.1 Import Character Mesh

```mel
File → Import (or drag FBX into scene)
// Mesh should be imported at same origin
// Mesh should align with skeleton in T-pose
```

### 4.2 Bind Skin to Skeleton

```mel
// Select mesh, then Shift+select root joint
Skin → Bind Skin → Bind Skin Options
  ├─ Bind To: Hierarchy
  ├─ Bind To: All Joints
  ├─ Max Influences: 4 (or 8 for complex areas)
  ├─ Falloff: Smooth Bind
  ├─ Weight Distribution: Distance
  └─ Apply
```

### 4.3 Paint Skin Weights

```mel
// Open Paint Weights tool
Skin → Paint Skin Weights

// For each joint, paint vertex influence:
// Left arm vertices: Weight to Arm_L, Forearm_L
// Left leg vertices: Weight to Thigh_L, Calf_L
// Torso vertices: Weight smoothly to Spine_01, Spine_02, Spine_03
// Head: Weight mostly to Head (Neck as falloff)

// Test deformations:
Select joint → Rotate (check for smooth bending)
```

### 4.4 Test Key Poses

```mel
// Create quick test animation to verify weights:

// Select Arm_L
// Key (S key) at frame 0 (rotated 0°)
// Move to frame 30
// Rotate Arm_L to 90° forward (pitch bend)
// Key (S key) at frame 30

// Play timeline: arm should bend smoothly with mesh deforming naturally
// Fix any sharp folds or collapsing geometry by adjusting skin weights
```

## Step 5: Clean Up & Prepare for Export

### 5.1 Clean Scene

```mel
Edit → Delete All by Type → History
Edit → Delete All by Type → Unknown Nodes
File → Optimize Scene Size
```

### 5.2 Verify Naming Convention

```mel
// Use script to rename all joints to match spec:
// No spaces, no special characters, _L/_R suffixes
// Verify with Outliner
```

### 5.3 Hide Control Rigs (if used)

```mel
// If you built control rigs, hide them on separate layer
// Display Layer: Hide non-skeleton layers before export
```

## Step 6: Export to FBX

### 6.1 Export Settings

```mel
File → Export Selection (or Export All if clean scene)

// Filename: SM_Streetballer_Base_Skeleton.fbx
// Location: SourceAssets/Exports/FBX/Characters/

// Export Options:
// ✅ Smooth Mesh Preview: OFF
// ✅ Skins: ON (smooth skin weights)
// ✅ Blend Shapes: ON (if morphs exist)
// ✅ Animation: OFF (skeleton only for now)
// ✅ Bake Animation: OFF
// ❌ Constraints: OFF (delete before export)
// ✅ Smoothing Groups: ON
// ✅ Hard Edges: ON

// Deformed Geometry: ON (if mesh included)
// Skins: ON
// Blend Shapes: ON
```

### 6.2 FBX File Details

```
Filename: SM_Streetballer_Base_Skeleton.fbx
Location: SourceAssets/Exports/FBX/Characters/
Format: FBX 2020 or 2024
Units: Centimeters (1:1 scale)
Axis: Y-Up, Z-Forward
Skeleton Bones: 61
Mesh Vertices: ~5000-8000 (typical)
```

## Step 7: Verify Export & Import into UE5

### 7.1 Import Skeleton into UE5

```
Content Browser → /Game/Blacktop/Art/Characters/Skeletons/
Right-click → Import
  ├─ Select: SM_Streetballer_Base_Skeleton.fbx
  ├─ Skeletal Mesh: Create New
  ├─ Skeleton: Create New (SK_Streetballer_Base)
  ├─ Materials: Create New
  └─ Import
```

### 7.2 Verify in UE5

```
Open SK_Streetballer_Base asset
  ├─ Skeleton Tree: Verify 61 bones
  ├─ Bone Names: Check all match specification
  ├─ T-Pose: Verify character is in T-pose
  ├─ Mesh Deformation: Select bone, rotate to verify skin deformation
  └─ Collisions: Physics Asset auto-created
```

## Troubleshooting

| Issue | Cause | Solution |
|-------|-------|----------|
| Arms twisted/deformed | Forearm twist bones missing | Verify all ArmTwist/ForearmTwist bones exist |
| Leg collapses when bent | Calf twist bone missing | Add CalfTwist_L_01/R_01 |
| Mesh detaches from bones | Skin weights not painted | Paint weights, ensure 100% coverage |
| Fingers don't move | Finger bones not in hierarchy | Check Hand_L/R has all 10 finger bones |
| Import fails in UE5 | FBX version issue | Re-export as FBX 2020 format |
| Character scale wrong | Unit mismatch | Set Maya to Centimeters, export 1:1 |

## Next Steps

1. ✅ **Complete this skeleton rig in Maya**
2. ⬜ **Create animation: Idle_Neutral (1-2 second loop)**
3. ⬜ **Create animation: Run_Forward (1-2 second loop)**
4. ⬜ **Export all animations as FBX**
5. ⬜ **Import into UE5 Animation Blueprint**
