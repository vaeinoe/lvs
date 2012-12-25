/*
 *  Toolbar.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#include "Toolbar.h"
#include "LVSEngine.h"
#include "Player.h"
#include "WProgressBar.h"
#include "../ciUI/ciUI.h"
#include <string>

double Toolbar::barColors[][4] = {
    {0.0, 0.0, 1.0, 0.5},
    {1.0, 1.0, 1.0, 0.5},
    {1.0, 0.0, 0.0, 0.5},
    {0.1, 1.0, 0.1, 0.5}
};

void Toolbar::setup( Configuration *config, const Vec2i loc, const Vec2i size )
{
    mConfig = config;

    int windowWidth = getWindowWidth();
    Vec2f firstPos = Vec2f(10, 5);
    Vec2f offset   = Vec2f(110, 0);
    Vec2f barSize  = Vec2f(100, 15);

    timerBar = new WProgressBar();
    timerBar->setup(Vec2f(windowWidth - 5, 20) - barSize, barSize, ColorA(0.5, 0.0, 0.5, 0.5), 100, 100);
    
    for (int i = 0; i < mConfig->numTileTypes; i++) {
        scoreBars[i] = new WProgressBar();
        ColorA barColor = ColorA(barColors[i][0], barColors[i][1],
                                 barColors[i][2], barColors[i][3]);
        scoreBars[i]->setup(firstPos + (i * offset), barSize, barColor, 100, 0);
    }
}

void Toolbar::updateScore( int score, int maxScore, int type, bool finished ) {
    if (finished) { // Bar grayed out
        scoreBars[type]->setColor(ColorA(1.0, 1.0, 1.0, 0.2));
        scoreBars[type]->setValue(maxScore, maxScore);
    }
    else {        
        scoreBars[type]->setValue(score, maxScore);
    }
}

void Toolbar::draw() {
    Vec2f start = Vec2f(0,0);
    Vec2f end   = Vec2f(getWindowWidth(), 25);
                        
    gl::color( 0.2, 0.0, 0.2, 0.33 );
    gl::drawSolidRect( Rectf(start, end) );
    
    timerBar->draw();
    for (int i = 0; i < mConfig->numTileTypes; i++) {
        scoreBars[i]->draw();
    }
}

void Toolbar::update( int fftDataBins )
{    
    timerBar->setValue(mConfig->engine->getGameTime(), mConfig->engine->getMaxTime());
}

void Toolbar::shutdown() { }
