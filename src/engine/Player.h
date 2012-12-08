/*
 *  Player.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */
#pragma once
#include "Configuration.h"
#include "Common.h"

class Configuration;

class Player {
public:
	void setup(Configuration *config);
	void update();
	void draw();
	void shutdown();
    void addScore(int score, int type);
    int getMaxScore( int type );
private:
    int mScores[64];
    Configuration *mConfig;
};
