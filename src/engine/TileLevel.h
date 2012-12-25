//
//  TileLevel.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/24/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"

class Configuration;

class TileLevel {
public:
    void setup(Configuration *config, int newType);
    void reset();
    void addScore(int newScore);

    int getScore();
    int getLevel();
    
    bool isFinished();
private:
    static int numLevels[TILE_TYPES];
    void levelUp();

    Configuration *mConfig;

    int type;
    int level;
    int score;
    int maxScore;
    
    int cumulScore;
    
    bool finished;
};
