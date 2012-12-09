/*
 *  Toolbar.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Toolbar.h"
#include <string>

void Toolbar::setup( Configuration *config, const Vec2i loc, const Vec2i size )
{
    mConfig = config;
    gui = new ciUICanvas(loc.x, loc.y, size.x, size.y);
    gui->setFont("nevis.ttf");
    
    gui->setFontSize(CI_UI_FONT_LARGE, 36);
    gui->setFontSize(CI_UI_FONT_MEDIUM, 24);
    gui->setFontSize(CI_UI_FONT_SMALL, 13);
    
    // Modified "Berlin"
    ColorA cb = ColorA( 0.1, 0.1, 0.1, 0.9 );
    ColorA co = ColorA( 0.294118, 0.34902, 0.419608, 0.392157 );
    ColorA coh = ColorA( 0.6, 0.894118, 1, 0.784314 );
    ColorA cf = ColorA( 0.968627, 0.309804, 0.309804, 0.784314 );
    ColorA cfh = ColorA( 1, 0.231373, 0.231373, 1 );
    ColorA cp = ColorA( 0.105882, 0.12549, 0.14902, 0.392157 );
    ColorA cpo = ColorA( 0.294118, 0.34902, 0.419608, 0.784314 ); 
    gui-> setUIColors( cb, co, coh, cf, cfh, cp, cpo );             
    

    gui->addWidgetRight(new ciUILabel("lvs",CI_UI_FONT_SMALL));
#ifdef DEBUG
    gui->addWidgetRight(new ciUIFPS(CI_UI_FONT_SMALL));
#endif

    for (int i = 0; i < mConfig->numTileTypes; i++) {
        char buf[100];
        sprintf(buf, "000/%03d", mConfig->player->getMaxScore(i));
        scoreLabels[i] = new ciUILabel(buf, CI_UI_FONT_SMALL);
        gui->addWidgetRight(scoreLabels[i]);
    }
    
    gui->autoSizeToFitWidgets();
    gui->registerUIEvents(this, &Toolbar::guiEvent);
}

void Toolbar::updateScore( int score, int maxScore, int type ) {
    char buf[100];
    sprintf(buf, "%03d/%03d", score, maxScore);
    scoreLabels[type]->setLabel(buf);
}

void Toolbar::shutdown() { delete gui; }
void Toolbar::draw() { gui->draw(); }

void Toolbar::update( int fftDataBins )
{
//    char buf[10];
//    snprintf(buf, 9, "%d", fftDataBins);
//    freqLabel->setLabel(buf);
	gui->update();
}

void Toolbar::guiEvent(ciUIEvent *event)
{
    // XXX
}
