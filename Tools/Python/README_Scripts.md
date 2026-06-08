# Blacktop Legends Production Scripts

Automation tools for character animation export/import workflow.

## Scripts

### 1. `export_animations_batch.py`

**Location**: `SourceAssets/Maya/`

**Purpose**: Export all 74 animations from Maya to FBX format automatically.

**Usage**:
```python
# In Maya Script Editor (Python tab):
exec(open('SourceAssets/Maya/export_animations_batch.py').read())
```

**Output**:
```
SourceAssets/Exports/FBX/Characters/
├── Idle/ (4 animations)
├── Movement/ (12 animations)
├── BallActions/ (21 animations)
├── Reactions/ (12 animations)
├── Celebrations/ (8 animations)
└── Transitions/ (6 animations)
```

**Features**:
- Automatically sets frame ranges for each animation
- Organizes exports by category
- Logs success/failure for each animation
- Creates directories if needed
- Uses proper FBX export settings (2020 format)

### 2. `import_animations_ue5.py`

**Location**: `Tools/Python/`

**Purpose**: Batch import 74 FBX animation files into UE5 with correct skeleton reference.

**Usage**:
```python
# In UE5 Python Console:
exec(open('Tools/Python/import_animations_ue5.py').read())
```

**Input**:
```
SourceAssets/Exports/FBX/Characters/
├── Idle/SM_*.fbx
├── Movement/SM_*.fbx
├── ... (etc)
```

**Output**:
```
/Game/Blacktop/Art/Characters/Animations/
├── Idle/
│   ├── Idle_Neutral_Anim
│   ├── Idle_Breathing_Anim
│   └── ...
├── Movement/
├── BallActions/
├── Reactions/
├── Celebrations/
└── Transitions/
```

**Features**:
- References `SK_Streetballer_Base` skeleton automatically
- Imports animations only (no mesh)
- Organizes by category folders
- Auto-names animations with `_Anim` suffix
- Logs success/failure
- Refreshes Content Browser

## Animation Definition Reference

Animations are defined in Python dictionaries:

```python
ANIMATIONS = {
    "Idle": [
        {"name": "Idle_Neutral", "start": 0, "end": 120, "loop": True},
        {"name": "Idle_Breathing", "start": 120, "end": 180, "loop": True},
        ...
    ],
    "Movement": [
        {"name": "Walk_Forward", "start": 500, "end": 540, "loop": True},
        ...
    ],
    ...
}
```

**Fields**:
- `name`: Animation identifier (matches UE5 naming)
- `start`: Start frame in Maya timeline
- `end`: End frame in Maya timeline
- `loop`: Whether animation is looping

## Workflow

### Complete Animation Production Loop

```
1. Create animation in Maya
   └─ Build keyframes from start to end frame
   
2. Export from Maya
   └─ Run: exec(open('export_animations_batch.py').read())
   
3. Verify exports
   └─ Check SourceAssets/Exports/FBX/Characters/[Category]/
   
4. Import into UE5
   └─ Run: exec(open('import_animations_ue5.py').read())
   
5. Test in-game
   └─ Verify animation plays in ABP_BLCharacter
```

## Troubleshooting

### Export fails in Maya
- **Issue**: "File not found" or permission error
- **Solution**: Check export path is writable, create directories manually
- **Check**: Maya FBX plugin is loaded (Windows → Preferences → Plug-in Manager)

### Import fails in UE5
- **Issue**: "Skeleton not found"
- **Solution**: Ensure `SK_Streetballer_Base` exists in `/Game/Blacktop/Art/Characters/Skeletons/`
- **Check**: FBX files exist in `SourceAssets/Exports/FBX/Characters/[Category]/`

### Animations in wrong folder
- **Issue**: Animations import to root instead of category folders
- **Solution**: Ensure source FBX files are in category subdirectories
- **Fix**: Run import script again after organizing FBX files

### Animation plays too fast/slow
- **Issue**: Frame rate mismatch
- **Solution**: Verify Maya scene is 30 FPS, UE5 animation is 30 FPS
- **Check**: Frame range in animation definition is correct

## Advanced Usage

### Export Single Animation

```python
# In Maya:
export_animation(
    anim_name="Custom_Animation",
    start_frame=1000,
    end_frame=1040,
    category="Custom",
    export_path="SourceAssets/Exports/FBX/Characters"
)
```

### Import Single Animation

```python
# In UE5:
fbx_path = "C:/Project/SourceAssets/Exports/FBX/Characters/Custom/SM_Custom_Animation.fbx"
task = create_animation_import_task(fbx_path, "Custom_Animation", "Custom")
unreal.AssetTools.get_asset_tools().import_asset_tasks([task])
```

### Batch Reimport (Update Existing)

```python
# Edit animation in Maya
# Export (overwrite FBX files)
# In UE5, run import script again
# All animations re-import with new data
```

## Performance

- Export all 74 animations: ~10-15 minutes in Maya
- Import all 74 animations: ~5-10 minutes in UE5
- Total disk space: ~240 MB (FBX files) + ~80 MB (imported animations)

## File Organization

```
project/
├── SourceAssets/
│   ├── Maya/
│   │   └── export_animations_batch.py
│   └── Exports/FBX/Characters/
│       ├── Idle/
│       ├── Movement/
│       ├── BallActions/
│       ├── Reactions/
│       ├── Celebrations/
│       └── Transitions/
├── Tools/
│   └── Python/
│       ├── import_animations_ue5.py
│       └── README_Scripts.md (this file)
└── Docs/
    └── Animation_Library_Plan.md
```

## Next Steps

1. ✅ Create skeleton in Maya
2. ✅ Define all 74 animations
3. 🔄 Create animations (in progress)
4. 🔄 Export using `export_animations_batch.py`
5. 🔄 Import using `import_animations_ue5.py`
6. ⬜ Set up Animation Blueprint
7. ⬜ Test in-game

---

**Questions?** See [`Complete_Implementation_Guide.md`](../Complete_Implementation_Guide.md)
