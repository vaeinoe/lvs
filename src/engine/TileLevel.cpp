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
#include "OverlayFxManager.h"

int TileLevel::numLevels[TILE_TYPES] = { 2, 2, 2, 2 };

// 0: blue
// 1: white
// 2: red
// 3: green

ColorA TileLevel::msgColors[TILE_TYPES] = {
    ColorA(0.6, 0.6, 1.0, 1.0),
    ColorA(0.9, 0.9, 0.9, 1.0),
    ColorA(1.0, 0.6, 0.6, 1.0),
    ColorA(0.6, 1.0, 0.6, 1.0),
};

std::string TileLevel::msgText[TILE_TYPES] = {
    "Blue",
    "White",
    "Red",
    "Green"
};

void TileLevel::setup(Configuration *config, int newType)
{
    mConfig = config;
    
    type  = newType;
    level = 0;
    
    cumulScore = 0;
    score      = 0;
    maxScore   = (level + 1) * LEVEL_SCORE_MODIFIER;
    
    finished = false;
}

void TileLevel::reset()
{
    level    = 0;
    score    = 0;
    maxScore = (level + 1) * LEVEL_SCORE_MODIFIER;
    cumulScore = 0;
    
    finished = false;
    mConfig->toolbar->updateScore(score, maxScore, level + 1, type, finished);
}

void TileLevel::addScore(int newScore)
{
    if (!finished) {
        score += newScore;
        if (score < 0) score = 0;
        if (score >= maxScore) levelUp();
        mConfig->toolbar->updateScore(score, maxScore, level + 1, type, finished);
    }
}

inline void TileLevel::levelUp()
{
    cumulScore += score;

    level += 1;
    score  = 0;
    maxScore = (level + 1) * LEVEL_SCORE_MODIFIER;
    mConfig->engine->addGameTime(maxScore);
    
    string lup;
    if (level == numLevels[type]) {
        finished = true;
        if (!mConfig->engine->isVictory()) {
            lup = msgText[type] + std::string(" complete.");            
        }
    }
    else {
        lup = msgText[type] + std::string(" level ") + toString(level + 1) + ".";
    }
    
    mConfig->toolbar->updateScore(score, maxScore, level + 1, type, finished);
    mConfig->overlayFx->createText(Vec2f(0,0), msgColors[type],
                                   FONT_TYPE_MEDIUM, lup);
}

bool TileLevel::isFinished() { return finished; }
int TileLevel::getScore() { return score; }
int TileLevel::getLevel() { return level; }

