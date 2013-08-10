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
#include "WTextLabel.h"
#include <string>

double Toolbar::barColors[][4] = {
    {0.0, 0.0, 1.0, 0.6},
    {1.0, 1.0, 1.0, 0.6},
    {1.0, 0.0, 0.0, 0.6},
    {0.1, 1.0, 0.1, 0.6}
};

void Toolbar::setup( Configuration *config, const Vec2i loc, const Vec2i size )
{
    mConfig = config;

    int windowWidth = mConfig->fieldSize.x;
    Vec2f levelLblPos   = Vec2f(10, 17);
    
    Vec2f firstPos      = Vec2f(70, 5);
    Vec2f offset        = Vec2f(140, 0);
    Vec2f labelOffset   = Vec2f(105, 12);
    Vec2f barSize       = Vec2f(100, 15);

    Vec2f timerLblOffset = Vec2f(-40, 11);
    Vec2f timerPos       = Vec2f(windowWidth - 5, 20) - barSize;
    
    timerLabel = new WTextLabel();
    timerLabel->setup(mConfig, timerPos + timerLblOffset, "time", FONT_TYPE_SMALL, ColorA(1.0, 1.0, 1.0, 0.4));
    
    timerBar = new WProgressBar();
    timerBar->setup(mConfig, timerPos, barSize, ColorA(0.6, 0.0, 0.5, 0.6), 100, 100);
    
    levelLabel = new WTextLabel();
    levelLabel->setup(mConfig, levelLblPos, "levels", FONT_TYPE_SMALL, ColorA(1.0, 1.0, 1.0, 0.4));
    
    for (int i = 0; i < mConfig->numTileTypes; i++) {
        scoreBars[i] = new WProgressBar();
        ColorA barColor = ColorA(barColors[i][0], barColors[i][1],
                                 barColors[i][2], barColors[i][3]);
        scoreBars[i]->setup(mConfig, firstPos + (i * offset), barSize, barColor, 100, 0);

        levelLabels[i] = new WTextLabel();
        levelLabels[i]->setup(mConfig, firstPos + (i * offset + labelOffset), "1", FONT_TYPE_SMALL, ColorA(1.0, 1.0, 1.0, 0.4));
    }
}

void Toolbar::reset() {
    Vec2f firstPos      = Vec2f(70, 5);
    Vec2f offset        = Vec2f(140, 0);
    Vec2f labelOffset   = Vec2f(105, 12);
    Vec2f barSize       = Vec2f(100, 15);

    for (int i = 0; i < mConfig->numTileTypes; i++) {
        ColorA barColor = ColorA(barColors[i][0], barColors[i][1],
                                 barColors[i][2], barColors[i][3]);
        scoreBars[i]->setup(mConfig, firstPos + (i * offset), barSize, barColor, 100, 0);
        
        levelLabels[i]->setup(mConfig, firstPos + (i * offset + labelOffset), "1", FONT_TYPE_SMALL, ColorA(1.0, 1.0, 1.0, 0.4));
    }
}

void Toolbar::updateScore( int score, int maxScore, int level, int type, bool finished ) {
    if (finished) { // Bar grayed out
        scoreBars[type]->setColor(ColorA(1.0, 1.0, 1.0, 0.2));
        scoreBars[type]->setValue(maxScore, maxScore);
        levelLabels[type]->setValue("--");
    }
    else {        
        scoreBars[type]->setValue(score, maxScore);
        levelLabels[type]->setValue(toString(level));
    }
}

void Toolbar::draw() {
    Vec2f start = mConfig->fieldOrigin + Vec2f(0,0);
    Vec2f end   = mConfig->fieldOrigin + Vec2f(mConfig->fieldSize.x, 25);
                        
    gl::color( 0.2, 0.0, 0.2, 0.33 );
    gl::drawSolidRect( Rectf(start, end) );

    levelLabel->draw();
    timerLabel->draw();
    timerBar->draw();
    for (int i = 0; i < mConfig->numTileTypes; i++) {
        scoreBars[i]->draw();
        levelLabels[i]->draw();
    }
}

void Toolbar::update()
{    
    timerBar->setValue(mConfig->engine->getGameTime(), mConfig->engine->getMaxTime());
}

void Toolbar::shutdown() { }
