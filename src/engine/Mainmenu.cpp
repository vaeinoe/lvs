//
//  Mainmenu.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 11/19/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "Mainmenu.h"
#include "LVSEngine.h"
#include "WTextLabel.h"
#include "WTextButton.h"

void Mainmenu::setup(Configuration *config)
{
    mConfig = config;
    
    title = new WTextLabel();
    title->setup(mConfig, Vec2f(16,64), "8trak", FONT_TYPE_LARGE, ColorA(0.6, 0.4, 0.6, 0.75));

    buttonStart = new WTextButton();
    buttonStart->setup(mConfig, Vec2f(16,110), "play", FONT_TYPE_MEDIUM, ColorA(0.8, 0.5, 0.8, 0.66));

    buttonQuit = new WTextButton();
    buttonQuit->setup(mConfig, Vec2f(16,148), "quit", FONT_TYPE_MEDIUM, ColorA(0.8, 0.5, 0.8, 0.66));
    
    active = true;
}

void Mainmenu::draw() {
    title->draw();
    buttonStart->draw();
    buttonQuit->draw();
}

void Mainmenu::update() { }

void Mainmenu::shutdown() {
    delete title;
    delete buttonStart;
    delete buttonQuit;
}

void Mainmenu::mouseMove ( const MouseEvent event )
{
    if (active) {
        Vec2f pos = event.getPos();
        buttonStart->mousePos(pos);
        buttonQuit->mousePos(pos);        
    }
}

void Mainmenu::mouseDown ( const MouseEvent event )
{
    if (active) {        
        Vec2f pos = event.getPos();
        if (buttonStart->isHit(pos)) {
            mConfig->engine->startGame();
        }
        else if (buttonQuit->isHit(pos)) {
            mConfig->engine->quitGame();
        }
    }
}

void Mainmenu::activate() { active = true; }
void Mainmenu::deactivate() { active = false; }
