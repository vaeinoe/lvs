//
//  Configuration.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 11/9/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Common.h"

class Player;
class World;
class LVSEngine;
class Toolbar;
class FaderPack;
class Audio;
class OverlayFxManager;
class TileLevel;

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
    
    Vec2f fieldOrigin;
    Vec2i fieldSize;
    Rectf fieldRect;
    
    TileLevel *levels[TILE_TYPES];
    
    Player *player;
    World *world;
    LVSEngine *engine;
    Toolbar *toolbar;
    FaderPack *faders;
    Audio *audio;
    OverlayFxManager *overlayFx;
};
