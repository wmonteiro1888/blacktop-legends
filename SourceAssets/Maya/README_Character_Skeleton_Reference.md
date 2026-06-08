# Blacktop Legends Character Skeleton Reference

## Overview

This document provides the complete joint hierarchy, naming conventions, and specifications for the Blacktop Legends streetballer character skeleton in Maya.

## Target Specifications

- **Total Bones**: 61 joints
- **Pose**: T-pose (arms at 90 degrees)
- **Scale**: 1 unit = 1 cm (180cm tall character ≈ 180 units)
- **Up Axis**: Y-axis up
- **Forward Direction**: Positive X-axis
- **Export Format**: FBX 2020 or higher
- **Engine Target**: Unreal Engine 5.7

## Complete Joint Hierarchy

```
root
├── Pelvis (0, 90, 0) [Hip center]
│
├── [LEFT SIDE]
│   ├── Thigh_L (rotate X=0, Y=0, Z=0)
│   │   ├── ThighTwist_L_01
│   │   ├── Calf_L
│   │   │   ├── CalfTwist_L_01
│   │   │   ├── Foot_L
│   │   │   │   ├── Ball_L
│   │   │   │   │   └── Toe_L_01
│   │   │   │   └── [IK Handle Target]
│   │   └── [IK Pole Vector - Knee_L]
│   │
│   └── Spine_01 [Lower torso]
│       ├── Spine_02 [Mid torso]
│       │   ├── Spine_03 [Upper torso/chest]
│       │   │   ├── Neck_01
│       │   │   │   └── Head
│       │   │   │
│       │   │   ├── Shoulder_L
│       │   │   │   ├── Arm_L
│       │   │   │   │   ├── ArmTwist_L_01
│       │   │   │   │   ├── ArmTwist_L_02
│       │   │   │   │   ├── Forearm_L
│       │   │   │   │   │   ├── ForearmTwist_L_01
│       │   │   │   │   │   ├── ForearmTwist_L_02
│       │   │   │   │   │   ├── Hand_L
│       │   │   │   │   │   │   ├── Thumb_L_01
│       │   │   │   │   │   │   │   └── Thumb_L_02
│       │   │   │   │   │   │   ├── Index_L_01
│       │   │   │   │   │   │   │   └── Index_L_02
│       │   │   │   │   │   │   ├── Middle_L_01
│       │   │   │   │   │   │   │   └── Middle_L_02
│       │   │   │   │   │   │   ├── Ring_L_01
│       │   │   │   │   │   │   │   └── Ring_L_02
│       │   │   │   │   │   │   ├── Pinky_L_01
│       │   │   │   │   │   │   │   └── Pinky_L_02
│       │   │   │   │   │   │   └── Index_L_Socket [Ball socket]
│       │   │   │   │   │   └── [IK Handle Target]
│       │   │   │   │   └── [IK Pole Vector - Elbow_L]
│
├── [RIGHT SIDE - Mirror of LEFT]
│   ├── Thigh_R
│   │   ├── ThighTwist_R_01
│   │   ├── Calf_R
│   │   │   ├── CalfTwist_R_01
│   │   │   ├── Foot_R
│   │   │   │   ├── Ball_R
│   │   │   │   │   └── Toe_R_01
│   │   │   │   └── [IK Handle Target]
│   │   └── [IK Pole Vector - Knee_R]
│   │
│   └── Shoulder_R
│       ├── Arm_R
│       │   ├── ArmTwist_R_01
│       │   ├── ArmTwist_R_02
│       │   ├── Forearm_R
│       │   │   ├── ForearmTwist_R_01
│       │   │   ├── ForearmTwist_R_02
│       │   │   ├── Hand_R
│       │   │   │   ├── Thumb_R_01
│       │   │   │   │   └── Thumb_R_02
│       │   │   │   ├── Index_R_01
│       │   │   │   │   └── Index_R_02
│       │   │   │   ├── Middle_R_01
│       │   │   │   │   └── Middle_R_02
│       │   │   │   ├── Ring_R_01
│       │   │   │   │   └── Ring_R_02
│       │   │   │   ├── Pinky_R_01
│       │   │   │   │   └── Pinky_R_02
│       │   │   │   └── Index_R_Socket
│       │   │   └── [IK Handle Target]
│       │   └── [IK Pole Vector - Elbow_R]
```

## Joint Specifications

### Pelvis (Root)
- **Position**: (0, 0, 0) - World origin
- **Rotation**: (0, 0, 0)
- **Purpose**: Hip center, primary locomotion driver
- **Children**: Thigh_L, Thigh_R, Spine_01

### Spine Chain (5 bones)

| Joint | Position Offset | Rotation | Notes |
|-------|-----------------|----------|-------|
| Spine_01 | (0, 10, 0) | (0, 0, 0) | Lower torso, connects to pelvis |
| Spine_02 | (0, 15, 0) | (0, 0, 0) | Mid torso |
| Spine_03 | (0, 15, 0) | (0, 0, 0) | Upper chest, shoulder anchor |
| Neck_01 | (0, 8, 0) | (0, 0, 0) | Head connector |
| Head | (0, 10, 0) | (0, 0, 0) | Head top |

### Left Leg (6 bones)

| Joint | Position Offset | Rotation | Axis Preference |
|-------|-----------------|----------|------------------|
| Thigh_L | (-8, -10, 0) | (0, 0, 0) | X=bend, Y=twist, Z=side |
| ThighTwist_L_01 | (0, -15, 0) | (0, 0, 0) | Twist relief, 1/3 down |
| Calf_L | (0, -35, 0) | (0, 0, 0) | Knee joint |
| CalfTwist_L_01 | (0, -15, 0) | (0, 0, 0) | Twist relief, 1/3 down |
| Foot_L | (0, -35, 0) | (0, 0, 0) | Ankle joint |
| Ball_L | (0, -8, 3) | (0, 0, 0) | Ball of foot |
| Toe_L_01 | (0, -3, 0) | (0, 0, 0) | Toe point |

### Right Leg (6 bones) - Mirror

Same offsets as Left Leg, but X position is positive (8, not -8).

### Left Arm (16 bones)

| Joint | Position Offset | Rotation | Notes |
|-------|-----------------|----------|-------|
| Shoulder_L | (-18, 0, 0) | (0, 0, 0) | Connects to Spine_03 |
| Arm_L | (-15, 0, 0) | (0, 0, 0) | Upper arm, shoulder-to-elbow |
| ArmTwist_L_01 | (0, -8, 0) | (0, 0, 0) | Twist relief, 1/3 down |
| ArmTwist_L_02 | (0, -8, 0) | (0, 0, 0) | Twist relief, 2/3 down |
| Forearm_L | (0, -18, 0) | (0, 0, 0) | Elbow-to-wrist |
| ForearmTwist_L_01 | (0, -6, 0) | (0, 0, 0) | Twist relief, 1/3 down |
| ForearmTwist_L_02 | (0, -6, 0) | (0, 0, 0) | Twist relief, 2/3 down |
| Hand_L | (0, -12, 0) | (0, 0, 0) | Wrist/hand base |
| Thumb_L_01 | (2, 0, 3) | (0, 0, 0) | Thumb knuckle 1 |
| Thumb_L_02 | (1, 0, 2) | (0, 0, 0) | Thumb knuckle 2 (tip) |
| Index_L_01 | (3, 0, -1) | (0, 0, 0) | Index knuckle 1 |
| Index_L_02 | (1, 0, -1) | (0, 0, 0) | Index knuckle 2 (tip) |
| Middle_L_01 | (2, 0, -2) | (0, 0, 0) | Middle knuckle 1 |
| Middle_L_02 | (1, 0, -2) | (0, 0, 0) | Middle knuckle 2 (tip) |
| Ring_L_01 | (1, 0, -3) | (0, 0, 0) | Ring knuckle 1 |
| Ring_L_02 | (1, 0, -3) | (0, 0, 0) | Ring knuckle 2 (tip) |
| Pinky_L_01 | (0, 0, -4) | (0, 0, 0) | Pinky knuckle 1 |
| Pinky_L_02 | (0, 0, -4) | (0, 0, 0) | Pinky knuckle 2 (tip) |
| Index_L_Socket | (3, 0, -1) | (0, 0, 0) | Ball socket for hands-free ball handling |

### Right Arm (16 bones) - Mirror

Same structure as Left Arm, but all X offsets are negated.

## Naming Convention Rules

### Format
- **Standard joints**: `[BoneName]_[LR]_[##]`
  - Example: `Arm_L_01`, `Finger_R_02`
  
- **Non-suffixed joints**: Single instance (no duplicate)
  - Example: `Pelvis`, `Spine_01`, `Head`

- **Twist relief bones**: Always numbered sequentially
  - Example: `ThighTwist_L_01`, `ThighTwist_L_02`

- **IK targets**: `IK_[LimbName]_[LR]`
  - Example: `IK_Foot_L`, `IK_Hand_L`

### No Spaces, Special Characters
- ✅ `Arm_L`
- ❌ `Arm L` (space)
- ❌ `Arm-L` (dash)
- ❌ `ArmL` (missing underscore)

## Export Checklist

Before exporting to FBX:

- [ ] All joint names match this specification
- [ ] No duplicate joint names
- [ ] Skeleton is in T-pose with zero rotations
- [ ] All transforms are frozen (or at bind pose)
- [ ] No animation keys present
- [ ] No constraints (or constraints are baked/deleted)
- [ ] Scene has no extra objects (clean workspace)
- [ ] Joint hierarchy is complete and unbroken
- [ ] All twist relief joints are present
- [ ] Character is 180 units tall (180 cm)
- [ ] Character faces positive X-axis
- [ ] No negative scale values

## UE5 Import Notes

When importing this skeleton into UE5:

1. **Skeleton Asset**: Create `SK_Streetballer_Base`
2. **Skeletal Mesh**: Import character mesh bound to this skeleton
3. **Physics Asset**: Auto-generate for ragdoll support
4. **Retargeting**: Use "Use in Retargeting" flag for future animation library reuse
5. **Bone Names**: UE5 will preserve all joint names; verify in Persona editor
