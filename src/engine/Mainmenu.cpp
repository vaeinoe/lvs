//
//  Mainmenu.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 11/19/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "Mainmenu.h"

void Mainmenu::setup(Configuration *config)
{
    mConfig = config;

    int windowWidth = getWindowWidth();
    int windowHeight = getWindowHeight();
    
    int x = windowWidth * 0.75;
    int y = windowHeight * 0.5;
    int width = 300;
    int height = 300;
    
    gui = new ciUICanvas(x, y, width, height);
    gui->setFont("nevis.ttf");
    
    gui->setFontSize(CI_UI_FONT_LARGE, 72);
    gui->setFontSize(CI_UI_FONT_MEDIUM, 24);
    gui->setFontSize(CI_UI_FONT_SMALL, 16);
    
    // Modified "Berlin"
    ColorA cb = ColorA( 0.1, 0.1, 0.1, 0.0 );
    ColorA co = ColorA( 0.294118, 0.34902, 0.419608, 0.392157 );
    ColorA coh = ColorA( 0.6, 0.894118, 1, 0.784314 );
    ColorA cf = ColorA( 0.968627, 0.309804, 0.309804, 0.784314 );
    ColorA cfh = ColorA( 1, 0.231373, 0.231373, 1 );
    ColorA cp = ColorA( 0.105882, 0.12549, 0.14902, 0.392157 );
    ColorA cpo = ColorA( 0.294118, 0.34902, 0.419608, 0.784314 );
    gui-> setUIColors( cb, co, coh, cf, cfh, cp, cpo );

    gui->addWidgetDown(new ciUILabel("lvs",CI_UI_FONT_LARGE));
    gui->addWidgetDown(new ciUILabelToggle(false, "play",CI_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ciUILabelToggle(false, "quit",CI_UI_FONT_MEDIUM));
    
    gui->autoSizeToFitWidgets();
    gui->registerUIEvents(this, &Mainmenu::guiEvent);
    
    active = true;
}

void Mainmenu::update()   { gui->update(); }
void Mainmenu::draw()     { gui->draw(); }
void Mainmenu::shutdown() { delete gui; }

void Mainmenu::mouseMove ( const MouseEvent event ) { }
void Mainmenu::mouseDown ( const MouseEvent event ) { }

void Mainmenu::activate() { active = true; }
void Mainmenu::deactivate() { active = false; }

void Mainmenu::guiEvent(ciUIEvent *event)
{
    if (active) {
        string name = event->widget->getName();
        if(name == "play") {
            mConfig->engine->startGame();
        }
        else if (name == "quit") {
            mConfig->engine->quitGame();
        }
    }
}
