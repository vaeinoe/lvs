//
//  TileLevel.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/24/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"

// Update score
// Get score

class Configuration;

class TileLevel {
public:
    void setup(Configuration *config, int newType);
    void reset();
    void addScore(int newScore);

    int getScore();
    int getLevel();
private:
    Configuration *mConfig;

    int type;
    int level;
    int score;
    int maxScore;
};
