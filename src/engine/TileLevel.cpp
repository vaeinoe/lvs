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
#include "AudioEngine.h"

int TileLevel::numLevels[TILE_TYPES] = { 4, 4, 4, 4 };

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
    "blue",
    "white",
    "red",
    "green"
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
        mConfig->audio->e_scoreChange(newScore, score);
    }
}

inline void TileLevel::levelUp()
{
    int level_times[4] = {40,30,30,40};

    cumulScore += score;

    level += 1;
    score  = 0;
    maxScore = (level + 1) * LEVEL_SCORE_MODIFIER;
    
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
    
    if (level <= numLevels[type]) mConfig->engine->addGameTime(level_times[level-1]);

    mConfig->toolbar->updateScore(score, maxScore, level + 1, type, finished);
    mConfig->overlayFx->createText(Vec2f(0,0), msgColors[type],
                                   FONT_TYPE_MEDIUM, lup);
    
    mConfig->audio->e_levelUp(type, level);
}

bool TileLevel::isFinished() { return finished; }
int TileLevel::getScore() { return score; }
int TileLevel::getLevel() { return level; }

