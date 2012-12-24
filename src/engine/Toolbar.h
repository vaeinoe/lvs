/*
 *  Toolbar.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */
#pragma once
#include "Configuration.h"

class ciUICanvas;
class ciUILabel;
class ciUIEvent;

class Configuration;
class WProgressBar;

class Toolbar {
public:
	void setup( Configuration *config, const Vec2i loc, const Vec2i size );
	void update( int fftDataBins );
	void draw();
	void shutdown();
	void guiEvent( ciUIEvent *event );
    void updateScore( int score, int maxScore, int type );
private:
    static double barColors[][4];
    
    Configuration *mConfig;
	ciUICanvas *gui;
    ciUILabel *scoreLabels[32];
    ciUILabel *timerLabel;
    
    WProgressBar *scoreBars[TILE_TYPES];
    WProgressBar *timerBar;
};
