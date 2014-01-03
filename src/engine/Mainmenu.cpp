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
//    title->setup(mConfig, Vec2f(16,1+164), GAME_NAME, FONT_TYPE_LARGE,
//                 ColorA(0.7, 0.5, 0.7, 0.80), true);
    title->setup(mConfig, Vec2f(16,1+164), GAME_NAME, FONT_TYPE_LARGE,
                 ColorA(0.7, 0.5, 0.7, 0.80), false);

/*    buttonStart = new WTextButton();
    buttonStart->setup(mConfig, Vec2f(16,47+164), "play", FONT_TYPE_MEDIUM,
                       ColorA(0.8, 0.5, 0.8, 0.66), true); */
    buttonStart = new WTextButton();
    buttonStart->setup(mConfig, Vec2f(16,47+164), "play", FONT_TYPE_MEDIUM,
                       ColorA(0.8, 0.5, 0.8, 0.66), false);

/*    buttonQuit = new WTextButton();
    buttonQuit->setup(mConfig, Vec2f(16,85+164), "quit", FONT_TYPE_MEDIUM,
                      ColorA(0.8, 0.5, 0.8, 0.66), true); */
    buttonQuit = new WTextButton();
    buttonQuit->setup(mConfig, Vec2f(16,85+164), "quit", FONT_TYPE_MEDIUM,
                      ColorA(0.8, 0.5, 0.8, 0.66), false);
    
    active = true;
}

void Mainmenu::draw() {
    title->draw();
    buttonStart->draw();
    buttonQuit->draw();

//    gl::color( ColorA(0.8, 0.5, 0.8, 0.66) );
//    gl::drawSolidRect ( Rectf(mConfig->fieldOrigin.x, mConfig->fieldOrigin.y,
//                               mConfig->fieldOrigin.x + mConfig->fieldSize.x,
//                               mConfig->fieldOrigin.y + 75) );
}

void Mainmenu::update() { }

void Mainmenu::shutdown() {
    delete title;
    delete buttonStart;
    delete buttonQuit;
}

void Mainmenu::setPaused ( bool paused ) {
    if (paused == true) { buttonStart->setValue("resume"); }
    else { buttonStart->setValue("play"); }
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
