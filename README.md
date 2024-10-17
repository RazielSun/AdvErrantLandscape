# AdvErrantLandscape Plugin

## Feature 1 - Spline Brushes
We got SDF map from spline with paramaters and apply it to Unreal Landscape.

+ AELHillBrushActor - Hill as Closed Spline
+ AELRoadBrushActor - Road as Basic Spline

## Feature 2
SDF shader uses Newton's algorithm based on gradient detection.

## How to use
* Go to Edit Mode -> Errant Landscape
* Select Add Button - Choose Brush (Hill or Road)
* Place it on the ground

## Requirements
+ Unreal 5.3.2
+ ErrantLandscape Plugin 1.4.0

## Issues
~~Spline SDF is brute force algorithm (need more optimized cubic spline get closest point)~~
+ It uses V0 (Depricated), because V1 uses some private function inside EL Plugin