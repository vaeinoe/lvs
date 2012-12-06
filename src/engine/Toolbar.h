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

class Configuration;

class Toolbar {
public:
	void setup( Configuration *config, const Vec2i loc, const Vec2i size );
	void update( int fftDataBins );
	void draw();
	void shutdown();
	void guiEvent( ciUIEvent *event );
    void updateScore( int score );
private:
    Configuration *mConfig;
	ciUICanvas *gui;
    ciUILabel *scoreLabel;
};
