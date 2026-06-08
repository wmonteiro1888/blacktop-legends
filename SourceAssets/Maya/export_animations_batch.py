#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Blacktop Legends - Maya Animation Batch Export Script

Purpose:
  Automate export of all 74 animations from Maya to FBX format.
  Organize exports into category folders matching UE5 structure.

Usage:
  1. Open Maya
  2. Open the character scene with all animations
  3. Script → Open Script Editor
  4. Copy this script into the Python tab
  5. Run script
  6. Select export directory (should default to SourceAssets/Exports/FBX/Characters/)
  7. All animations export automatically

Output:
  FBX files in SourceAssets/Exports/FBX/Characters/[Category]/
  - Idle/
  - Movement/
  - BallActions/
  - Reactions/
  - Celebrations/
  - Transitions/
"""

import os
import maya.cmds as cmds
import maya.mel as mel
from pathlib import Path

# Animation definitions matching the Animation Library Plan
ANIMATIONS = {
    "Idle": [
        {"name": "Idle_Neutral", "start": 0, "end": 120, "loop": True},
        {"name": "Idle_Breathing", "start": 120, "end": 180, "loop": True},
        {"name": "Idle_Tired", "start": 180, "end": 270, "loop": True},
        {"name": "Idle_Defense", "start": 270, "end": 370, "loop": True},
    ],
    "Movement": [
        {"name": "Walk_Forward", "start": 500, "end": 540, "loop": True},
        {"name": "Walk_Backward", "start": 540, "end": 580, "loop": True},
        {"name": "Walk_Left", "start": 580, "end": 620, "loop": True},
        {"name": "Walk_Right", "start": 620, "end": 660, "loop": True},
        {"name": "Run_Forward", "start": 700, "end": 730, "loop": True},
        {"name": "Run_Backward", "start": 730, "end": 760, "loop": True},
        {"name": "Run_Left", "start": 760, "end": 790, "loop": True},
        {"name": "Run_Right", "start": 790, "end": 820, "loop": True},
        {"name": "Sprint_Forward", "start": 850, "end": 874, "loop": True},
        {"name": "Sprint_Backward", "start": 874, "end": 898, "loop": True},
        {"name": "Sprint_Left", "start": 898, "end": 922, "loop": True},
        {"name": "Sprint_Right", "start": 922, "end": 946, "loop": True},
    ],
    "BallActions": [
        {"name": "Pickup_Ground_Front", "start": 1000, "end": 1020, "loop": False},
        {"name": "Pickup_Ground_Side", "start": 1020, "end": 1040, "loop": False},
        {"name": "Pickup_Pass_Catch", "start": 1040, "end": 1055, "loop": False},
        {"name": "Release_Idle", "start": 1055, "end": 1065, "loop": False},
        {"name": "Release_Ground", "start": 1065, "end": 1073, "loop": False},
        {"name": "Dribble_Idle", "start": 1100, "end": 1140, "loop": True},
        {"name": "Dribble_Walk", "start": 1140, "end": 1180, "loop": True},
        {"name": "Dribble_Run", "start": 1180, "end": 1210, "loop": True},
        {"name": "Dribble_Crossover", "start": 1210, "end": 1240, "loop": False},
        {"name": "Dribble_Behind", "start": 1240, "end": 1265, "loop": False},
        {"name": "Dribble_Hesitation", "start": 1265, "end": 1285, "loop": False},
        {"name": "Shoot_Jump_Mid", "start": 1300, "end": 1340, "loop": False},
        {"name": "Shoot_Jump_3Pt", "start": 1340, "end": 1385, "loop": False},
        {"name": "Shoot_Layup_Right", "start": 1385, "end": 1415, "loop": False},
        {"name": "Shoot_Layup_Left", "start": 1415, "end": 1445, "loop": False},
        {"name": "Shoot_Dunk_Start", "start": 1445, "end": 1465, "loop": False},
        {"name": "Shoot_Dunk_Finish", "start": 1465, "end": 1480, "loop": False},
        {"name": "Pass_Chest", "start": 1500, "end": 1518, "loop": False},
        {"name": "Pass_Bounce", "start": 1518, "end": 1536, "loop": False},
        {"name": "Pass_Overhead", "start": 1536, "end": 1554, "loop": False},
        {"name": "Pass_Behind", "start": 1554, "end": 1576, "loop": False},
    ],
    "Reactions": [
        {"name": "Jump_Vertical", "start": 1600, "end": 1640, "loop": False},
        {"name": "Jump_Forward", "start": 1640, "end": 1680, "loop": False},
        {"name": "Land_Soft", "start": 1680, "end": 1692, "loop": False},
        {"name": "Land_Hard", "start": 1692, "end": 1707, "loop": False},
        {"name": "Foul_Called", "start": 1707, "end": 1737, "loop": False},
        {"name": "Foul_Protest", "start": 1737, "end": 1782, "loop": False},
        {"name": "Miss_Shot", "start": 1782, "end": 1812, "loop": False},
        {"name": "Block_Reaction", "start": 1812, "end": 1837, "loop": False},
        {"name": "Steal_Reaction", "start": 1837, "end": 1857, "loop": False},
        {"name": "Exhausted", "start": 1857, "end": 1917, "loop": True},
        {"name": "Pain_Reaction", "start": 1917, "end": 1947, "loop": False},
        {"name": "Fall_Recovery", "start": 1947, "end": 1982, "loop": False},
    ],
    "Celebrations": [
        {"name": "Celebrate_Bucket", "start": 2000, "end": 2045, "loop": False},
        {"name": "Celebrate_3Pointer", "start": 2045, "end": 2095, "loop": False},
        {"name": "Celebrate_Dunk", "start": 2095, "end": 2150, "loop": False},
        {"name": "Celebrate_Team_High_Five", "start": 2150, "end": 2180, "loop": False},
        {"name": "Celebrate_Crowd_Pump", "start": 2180, "end": 2215, "loop": False},
        {"name": "Celebrate_Showboat", "start": 2215, "end": 2275, "loop": False},
        {"name": "Celebrate_Walk_Off", "start": 2275, "end": 2320, "loop": False},
        {"name": "Chant_Hype", "start": 2320, "end": 2360, "loop": True},
    ],
    "Transitions": [
        {"name": "Transition_RunToWalk", "start": 2400, "end": 2420, "loop": False},
        {"name": "Transition_WalkToRun", "start": 2420, "end": 2435, "loop": False},
        {"name": "Transition_RunToSprint", "start": 2435, "end": 2447, "loop": False},
        {"name": "Transition_SprintToRun", "start": 2447, "end": 2462, "loop": False},
        {"name": "Transition_DefenseStance", "start": 2462, "end": 2480, "loop": False},
        {"name": "Transition_ToShoot", "start": 2480, "end": 2492, "loop": False},
    ],
}

def export_animation(anim_name, start_frame, end_frame, category, export_path):
    """
    Export a single animation to FBX.
    
    Args:
        anim_name (str): Name of animation (e.g., "Idle_Neutral")
        start_frame (int): Start frame
        end_frame (int): End frame
        category (str): Category folder (e.g., "Idle")
        export_path (str): Base export directory
    
    Returns:
        bool: True if export successful
    """
    try:
        # Set frame range
        cmds.playbackOptions(minTime=start_frame, maxTime=end_frame)
        
        # Create category folder if it doesn't exist
        category_path = os.path.join(export_path, category)
        if not os.path.exists(category_path):
            os.makedirs(category_path)
        
        # Construct FBX filename
        fbx_filename = f"SM_{anim_name}.fbx"
        fbx_path = os.path.join(category_path, fbx_filename)
        
        # FBX export options
        mel_command = f"""
        FBXExportAnimationOnly -v true;
        FBXExportConstraints -v false;
        FBXExportSmoothingGroups -v true;
        FBXExportHardEdges -v false;
        FBXExportDeformedModels -v true;
        FBXExportSkins -v true;
        FBXExportBlendDeformations -v true;
        FBXExport -f \"{fbx_path}\" -s;
        """
        
        # Execute export
        mel.eval(mel_command)
        
        print(f"✓ Exported: {anim_name} ({start_frame}-{end_frame}) -> {fbx_filename}")
        return True
        
    except Exception as e:
        print(f"✗ Failed to export {anim_name}: {str(e)}")
        return False

def main():
    """
    Main export function.
    """
    # Determine export path
    export_path = os.path.expanduser("~/Blacktop_Legends/SourceAssets/Exports/FBX/Characters")
    
    # Allow user to override export path
    user_path = cmds.fileDialog2(
        caption="Select Export Directory",
        fileMode=2,
        dialogStyle=2
    )
    
    if user_path:
        export_path = user_path[0]
    
    print(f"\n=== Blacktop Legends Animation Export ===")
    print(f"Export Path: {export_path}\n")
    
    total_animations = 0
    successful_exports = 0
    
    # Export all animations
    for category, anims in ANIMATIONS.items():
        print(f"\n[{category}]")
        for anim in anims:
            total_animations += 1
            if export_animation(
                anim["name"],
                anim["start"],
                anim["end"],
                category,
                export_path
            ):
                successful_exports += 1
    
    # Summary
    print(f"\n=== Export Summary ===")
    print(f"Total Animations: {total_animations}")
    print(f"Successfully Exported: {successful_exports}")
    print(f"Failed: {total_animations - successful_exports}")
    print(f"\nExport Location: {export_path}")
    print("\n✓ Batch export complete!\n")

if __name__ == "__main__":
    main()
