/*
 *  Toolbar.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "Configuration.h"
#include "Common.h"
#include "../ciUI/ciUI.h"

class ciUICanvas;
class ciUILabel;
class ciUIEvent;

class Configuration;

class Toolbar {
public:
	void setup( Configuration *config, const Vec2i loc, const Vec2i size );
	void update( int fftDataBins );
	void draw();
	void shutdown();
	void guiEvent( ciUIEvent *event );
    void updateScore( int score, int maxScore, int type );
private:
    Configuration *mConfig;
	ciUICanvas *gui;
    ciUILabel *scoreLabels[32];
    ciUILabel *timerLabel;
};
