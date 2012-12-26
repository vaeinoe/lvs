//
//  TileLevel.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/24/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "TileLevel.h"
#include "Toolbar.h"
#include "LVSEngine.h"

int TileLevel::numLevels[TILE_TYPES] = { 2, 2, 2, 2 };

void TileLevel::setup(Configuration *config, int newType)
{
    mConfig = config;
    
    type  = newType;
    level = 0;
    
    cumulScore = 0;
    score      = 0;
    maxScore   = (level + 1) * 50;
    
    finished = false;
}

void TileLevel::reset()
{
    level    = 0;
    score    = 0;
    maxScore = (level + 1) * 50;
    cumulScore = 0;
    
    mConfig->toolbar->updateScore(score, maxScore, type, finished);
}

void TileLevel::addScore(int newScore)
{
    if (!finished) {
        score += newScore;
        if (score < 0) score = 0;
        if (score >= maxScore) levelUp();
        mConfig->toolbar->updateScore(score, maxScore, type, finished);
    }
}

inline void TileLevel::levelUp()
{
    cumulScore += score;

    level += 1;
    score  = 0;
    maxScore = (level + 1) * 50;
    mConfig->engine->addGameTime(maxScore);
    
    if (level == numLevels[type]) {
        finished = true;
    }
    
    mConfig->toolbar->updateScore(score, maxScore, type, finished);
}

bool TileLevel::isFinished() { return finished; }
int TileLevel::getScore() { return score; }
int TileLevel::getLevel() { return level; }

