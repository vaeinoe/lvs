/*
 *  Player.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#include "Player.h"

void Player::setup(Configuration *config)
{
    mConfig = config;
    for (int i = 0; i < mConfig->numTileTypes; i++) {
        mScores[i] = 0;
    }
}

int Player::getMaxScore(int type) {
    return 100;
}

void Player::addScore(int score, int type)
{
    mScores[type] += score;
    if (mScores[type] < 0) mScores[type] = 0;
    mConfig->toolbar->updateScore(mScores[type], 100, type);
}

void Player::update() { }

void Player::draw() { }

void Player::shutdown() { }
