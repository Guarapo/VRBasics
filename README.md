# VRBasics
VR Basic Movement 

Simple template game to make a locomotion movement on Oculus Quest

# Version

Unreal Engine 4.24.3

# Style and Code Standard

https://github.com/Allar/ue4-style-guide#0.1
https://docs.unrealengine.com/en-US/Programming/Development/CodingStandard/index.html#exampleformatting

## Inputs

TBD

## How to Install the game
Install via SideQuest https://sidequestvr.com/setup-howto 

## Game Architecture

All the gameplay was made on C++, only it uses blueprints on the UI and inputs that call game logic.

## Dependencies

- Oculus Plugin: https://developer.oculus.com/documentation/unreal/unreal-getting-started-guide/

## Folder Structure

```
.
├── Config
│   ├── DefaultEditor.ini
│   ├── DefaultEngine.ini
│   ├── DefaultGame.ini
│   └── DefaultInput.ini
├── Content
│   ├── Collections
│   ├── Developers
│   │   └── cris
│   └── VRBasicsCore
│       ├── Characters
│       ├── Core
│       ├── Maps
│       ├── Materials
│       └── UI
├── README.md
├── Source
│   ├── VRBasics
│   │   ├── Private
│   │   ├── Public
│   │   ├── VRBasics.Build.cs
│   │   ├── VRBasics.cpp
│   │   ├── VRBasics.h
│   │   ├── VRBasicsGameModeBase.cpp
│   │   └── VRBasicsGameModeBase.h
│   ├── VRBasics.Target.cs
│   └── VRBasicsEditor.Target.cs
├── VRBasics.uproject

```
