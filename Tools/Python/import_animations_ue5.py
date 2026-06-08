#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Blacktop Legends - UE5 Animation Batch Import Script

Purpose:
  Automate import of all 74 FBX animation files into UE5.
  Import to correct skeleton (SK_Streetballer_Base)
  Organize into correct folders in Content Browser

Usage:
  1. Open Blacktop Legends project in UE5 Editor
  2. Open Python console: Window → Developer Tools → Python Console
  3. Copy and execute this script
  4. All animations import automatically to:
     /Game/Blacktop/Art/Characters/Animations/[Category]/

Requirements:
  - UE5 Editor must be running
  - Python plugin enabled in UE5
  - SK_Streetballer_Base skeleton must exist
  - All FBX files in SourceAssets/Exports/FBX/Characters/[Category]/
"""

import unreal
import os
from pathlib import Path

# UE5 Settings
SKELETON_PATH = "/Game/Blacktop/Art/Characters/Skeletons/SK_Streetballer_Base"
ANIMATION_BASE_PATH = "/Game/Blacktop/Art/Characters/Animations"
SOURCE_FBX_PATH = "../SourceAssets/Exports/FBX/Characters"  # Relative to project

# Animation categories
CATEGORIES = [
    "Idle",
    "Movement",
    "BallActions",
    "Reactions",
    "Celebrations",
    "Transitions",
]

def create_animation_import_task(fbx_file_path, animation_name, category):
    """
    Create FBX import task for animation.
    
    Args:
        fbx_file_path (str): Full path to FBX file
        animation_name (str): Name of animation (e.g., "Idle_Neutral")
        category (str): Category folder (e.g., "Idle")
    
    Returns:
        unreal.AssetImportTask: Configured import task
    """
    # Create import task
    task = unreal.AssetImportTask()
    task.set_editor_property('filename', fbx_file_path)
    
    # Set destination path
    dest_path = f"{ANIMATION_BASE_PATH}/{category}"
    task.set_editor_property('destination_path', dest_path)
    
    # Set asset name
    task.set_editor_property('destination_name', f"{animation_name}_Anim")
    task.set_editor_property('replace_existing', True)
    task.set_editor_property('save', True)
    task.set_editor_property('automated', True)
    
    # FBX Import Options
    options = unreal.FbxImportUI()
    options.set_editor_property('import_mesh', False)  # Animation only
    options.set_editor_property('import_skeletal_mesh', False)
    options.set_editor_property('import_morph_targets', False)
    options.set_editor_property('import_animations', True)
    options.set_editor_property('import_materials', False)
    options.set_editor_property('import_textures', False)
    options.set_editor_property('create_physics_asset', False)
    
    # Skeletal options
    options.set_editor_property('skeleton', unreal.load_asset(SKELETON_PATH))
    options.set_editor_property('update_skeleton_reference_pose', False)
    options.set_editor_property('use_t0_as_ref_pose', False)
    options.set_editor_property('create_new_skeleton', False)
    
    task.set_editor_property('options', options)
    
    return task

def import_animations_from_directory():
    """
    Scan FBX directory and import all animations.
    """
    print("\n=== Blacktop Legends Animation Import ===")
    print(f"Skeleton: {SKELETON_PATH}")
    print(f"Destination: {ANIMATION_BASE_PATH}\n")
    
    # Get project directory
    project_dir = unreal.Paths.get_project_dir()
    source_base_path = os.path.join(project_dir, SOURCE_FBX_PATH)
    
    total_animations = 0
    successful_imports = 0
    
    # Import by category
    for category in CATEGORIES:
        category_path = os.path.join(source_base_path, category)
        
        if not os.path.exists(category_path):
            print(f"⚠ {category} folder not found: {category_path}")
            continue
        
        print(f"\n[{category}]")
        
        # Find all FBX files in category
        fbx_files = [f for f in os.listdir(category_path) if f.endswith('.fbx')]
        
        for fbx_file in sorted(fbx_files):
            fbx_path = os.path.join(category_path, fbx_file)
            
            # Extract animation name from filename (remove "SM_" prefix and ".fbx" suffix)
            animation_name = fbx_file.replace("SM_", "").replace(".fbx", "")
            
            total_animations += 1
            
            try:
                # Create and execute import task
                import_task = create_animation_import_task(fbx_path, animation_name, category)
                
                # Execute import
                unreal.AssetTools.get_asset_tools().import_asset_tasks([import_task])
                
                print(f"✓ Imported: {animation_name}")
                successful_imports += 1
                
            except Exception as e:
                print(f"✗ Failed to import {animation_name}: {str(e)}")
    
    # Summary
    print(f"\n=== Import Summary ===")
    print(f"Total Animations: {total_animations}")
    print(f"Successfully Imported: {successful_imports}")
    print(f"Failed: {total_animations - successful_imports}")
    print(f"\nImported to: {ANIMATION_BASE_PATH}")
    print("\n✓ Batch import complete!\n")

if __name__ == "__main__":
    # Execute import
    import_animations_from_directory()
    
    # Refresh Content Browser
    unreal.EditorAssetLibrary.rescan_hard_referenced_assets()
    print("Content Browser refreshed.")
