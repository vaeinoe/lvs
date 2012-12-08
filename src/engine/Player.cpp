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
    mScore = 0;
    mConfig = config;
}

void Player::addScore(int score)
{
    mScore += score;
    mConfig->toolbar->updateScore(mScore);
}

void Player::update() { }

void Player::draw() { }

void Player::shutdown() { }
