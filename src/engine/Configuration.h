//
//  Configuration.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 11/9/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Common.h"

class World;
class LVSEngine;
class Toolbar;
class FaderPack;
class AudioEngine;
class OverlayFxManager;
class TileLevel;
class HighScores;

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
    float padding_x;
    float padding_y;
    int solverDelayFrames;
    
    Vec2f fieldOrigin;
    Vec2i fieldSize;
    Rectf fieldRect;
    
    TileLevel *levels[TILE_TYPES];
    
    World *world;
    LVSEngine *engine;
    Toolbar *toolbar;
    FaderPack *faders;
    AudioEngine *audio;
    OverlayFxManager *overlayFx;
    HighScores *hiScores;
    
    gl::TextureFontRef fontSmall;
    gl::TextureFontRef fontMedium;
    gl::TextureFontRef fontLarge;
};
