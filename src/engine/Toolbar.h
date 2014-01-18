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
#include "cinder/gl/Fbo.h"

class Configuration;
class WProgressBar;
class WTextLabel;

class Toolbar {
public:
	void setup( Configuration *config, const Vec2i loc, const Vec2i size );
	void update();
	void draw();
	void shutdown();
    void reset();
    void updateScore( int score, int maxScore, int level, int type, bool finished = false );
private:
    static double barColors[][4];

    gl::Fbo toolbarFbo;
    void updateSurface();
    
    Configuration *mConfig;
    
    WTextLabel   *levelLabel;
    WProgressBar *scoreBars[TILE_TYPES];
    WTextLabel   *levelLabels[TILE_TYPES];
    
    WTextLabel   *timerLabel;
    WProgressBar *timerBar;
};
