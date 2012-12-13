//
//  Configuration.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 11/9/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Common.h"
#include "Player.h"
#include "World.h"
#include "Toolbar.h"
#include "Audio.h"
#include "LVSEngine.h"
#include "OverlayFxManager.h"

class Player;
class World;
class LVSEngine;
class Toolbar;
class FaderPack;
class Audio;

class Configuration {
public:
    int toolbarHeight;
    float tileSize;
    float tileUpperLimit;
    float tileGrid;
    float tileBorderSpacing;
    int worldWidth;
    int worldHeight;
    int numTileTypes;
    float padding;
    int solverDelayFrames;
    
    int tileLevels[TILE_TYPES];
    
    Player *player;
    World *world;
    LVSEngine *engine;
    Toolbar *toolbar;
    FaderPack *faders;
    Audio *audio;
    OverlayFxManager *overlayFx;
};
