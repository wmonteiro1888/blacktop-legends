# Blacktop Legends

**A 5v5 Street Basketball Game in Unreal Engine 5**

![Status](https://img.shields.io/badge/Status-First_Playable-orange)
![Engine](https://img.shields.io/badge/Engine-UE5.7-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B%2FBlueprint-brightgreen)

## Overview

Blacktop Legends is an arcade-realistic streetball game bringing the energy of iconic park runs like Harlem Crown to life. Play custom 5v5 matches, create your legend, and earn your spot at the park.

## Current Status

### ✅ Completed
- 5v5 multiplayer server foundation
- Harlem Crown park art pass (1,800+ Maya assets replaced)
- Character skeleton (61-bone rig)
- Animation library plan (74 animations)
- UE5 animation blueprint architecture

### 🚀 In Progress
- Character animation creation
- Animation blueprint implementation
- Player controller input setup
- Gameplay mechanics (shooting, passing, ball physics)

### 📋 Planned
- Online multiplayer matchmaking
- Customization system (outfits, stats)
- Progression & ranking
- Replay & highlight system
- Crowd reactions & atmosphere

## Quick Start

### Prerequisites
- Unreal Engine 5.7+
- Maya 2024+ (for animation development)
- Visual Studio 2022
- Python 3.9+ (for batch scripts)

### Setup

```bash
# Clone repository
git clone https://github.com/wmonteiro1888/blacktop-legends.git
cd blacktop-legends

# Open UE5 project
# File → Open Project → BlacktopLegends.uproject

# Build C++ project
Generate Visual Studio project files
Open BlacktopLegends.sln
Build Solution
```

## Documentation

### Character & Animation
- [`SourceAssets/Maya/README_Character_Skeleton_Reference.md`](SourceAssets/Maya/README_Character_Skeleton_Reference.md) - Complete 61-bone joint specification
- [`SourceAssets/Maya/README_Character_Rigging_Workflow.md`](SourceAssets/Maya/README_Character_Rigging_Workflow.md) - Step-by-step Maya rigging guide
- [`Docs/Animation_Library_Plan.md`](Docs/Animation_Library_Plan.md) - 74 animation specifications
- [`Docs/UE5_Animation_Blueprint_Setup.md`](Docs/UE5_Animation_Blueprint_Setup.md) - Animation Blueprint architecture
- [`Docs/Complete_Implementation_Guide.md`](Docs/Complete_Implementation_Guide.md) - End-to-end workflow guide

### Art & Production
- [`Docs/BlacktopLegends_FirstPlayable.md`](Docs/BlacktopLegends_FirstPlayable.md) - Project vision & current state
- [`Docs/BlacktopLegends_Maya_Asset_Manifest.md`](Docs/BlacktopLegends_Maya_Asset_Manifest.md) - Asset families & replacements

### Production Scripts
- [`SourceAssets/Maya/export_animations_batch.py`](SourceAssets/Maya/export_animations_batch.py) - Batch export 74 animations to FBX
- [`Tools/Python/import_animations_ue5.py`](Tools/Python/import_animations_ue5.py) - Batch import animations into UE5

### C++ Code
- [`Source/BlacktopLegends/Character/BLCharacterAnimInstance.h/cpp`](Source/BlacktopLegends/Character/) - Animation system
- [`Source/BlacktopLegends/Character/BLCharacterController.h/cpp`](Source/BlacktopLegends/Character/) - Player input controller

## Workflow

### Creating a New Animation

```
1. Maya: Create animation frames 1-X
2. Maya: Run export_animations_batch.py
3. UE5: Run import_animations_ue5.py
4. UE5: Add to ABP_BLCharacter state machine
5. Test in-game
```

### Animation Import Quick Commands

**Maya (Python Console):**
```python
# Export all animations
exec(open('export_animations_batch.py').read())
```

**UE5 (Python Console):**
```python
# Import all animations
exec(open('../Tools/Python/import_animations_ue5.py').read())
```

## Architecture

### Multiplayer Foundation
- Server-authoritative (BLGameMode)
- Player state replication (BLPlayerState)
- 5v5 team assignment with Home/Away designation
- Replicated ball possession and scoring

### Animation System
- 61-bone humanoid skeleton
- 74 pre-planned animations across 6 categories
- 2D blend spaces for directional movement
- State machine with 6 core states (Idle, Movement, BallHandling, Shooting, Reaction, Celebration)
- Montages for one-shot actions (shooting, passing)

### Character Controller
- Enhanced Input System (UE5 standard)
- 8-directional movement with blend spaces
- Contextual actions (jump, shoot, pass, dribble, sprint)
- Stamina system affecting animation speed

## Team

- **Developer**: wmonteiro1888
- **Project**: Blacktop Legends - First Playable
- **Timeline**: 4 weeks (June 2026)

## License

MIT License - See LICENSE file for details

## Contributing

Contributions welcome! See CONTRIBUTING.md for guidelines.

## Support

For questions or issues:
1. Check documentation in `/Docs/`
2. Review production guides in `/SourceAssets/Maya/`
3. Open GitHub issue with details

---

**Status**: 🟡 First Playable in Development
**Next Milestone**: Character animation & gameplay integration
