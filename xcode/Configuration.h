//
//  Configuration.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 11/9/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Common.h"

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
};
