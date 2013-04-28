//
//  HighScores.h
//  Leavs
//
//  Created by vae on 22/04/2013.
//  Copyright (c) 2013 Lumeet. All rights reserved.
//

#pragma once

#include "Configuration.h"

class HighScores {
public:
	void setup(Configuration *config);
	void shutdown();
    void saveScore(int score); // TODO: pass some kind of score object or player name and a bunch of scores?
private:
    Configuration *mConfig;
    bool active;
};

