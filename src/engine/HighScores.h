//
//  HighScores.h
//  Leavs
//
//  Created by vae on 22/04/2013.
//  Copyright (c) 2013 Lumeet. All rights reserved.
//

#pragma once

#include "Configuration.h"
#include "PreferenceWrapper.h"

#define DUMMY_SCORE 9999

class HighScores {
public:
	void setup(Configuration *config);
	void shutdown();
    void saveScore(int score);
    int getScore();
private:
    int topScore;

    string getFilePath();
    string xorString(string str);
    void loadScoreFile();
    void saveScoreFile();
    int decodeScore(string score_str);
    string encodeScore(int score);
    Configuration *mConfig;
    bool active;
};

