//
//  TileLevel.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/24/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "TileLevel.h"
#include "Toolbar.h"

void TileLevel::setup(Configuration *config, int newType)
{
    mConfig = config;
    
    type  = newType;
    level = 0;
    
    score    = 0;
    maxScore = (level + 1) * 50;
}

void TileLevel::reset()
{
    level    = 0;
    score    = 0;
    maxScore = (level + 1) * 50;
}

void TileLevel::addScore(int newScore)
{
    score += newScore;
    if (score < 0) score = 0;
    if (score >= maxScore) {
        level += 1;
        score  = 0;
        maxScore = (level + 1) * 50;
        // XXX levelwise max scores
    }
    mConfig->toolbar->updateScore(score, maxScore, type);
}

int TileLevel::getScore() { return score; }
int TileLevel::getLevel() { return level; }

