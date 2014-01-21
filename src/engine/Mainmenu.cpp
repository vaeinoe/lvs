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
#include "HighScores.h"

void Mainmenu::setup(Configuration *config)
{
    mConfig = config;
    
    title = new WTextLabel();
    title->setup(mConfig, Vec2f(16,1+164), GAME_NAME, FONT_TYPE_LARGE,
                 ColorA(0.7, 0.5, 0.7, 0.80), true);

    buttonStart = new WTextButton();
    buttonStart->setup(mConfig, Vec2f(16,47+164), "new game", FONT_TYPE_MEDIUM,
                       ColorA(0.8, 0.5, 0.8, 0.66), true);

    buttonQuit = new WTextButton();
    buttonQuit->setup(mConfig, Vec2f(16,85+164), "quit", FONT_TYPE_MEDIUM,
                      ColorA(0.8, 0.5, 0.8, 0.66), true);
    
    score = new WTextLabel();
    score->setup(mConfig, Vec2f(8,758), "best time:", FONT_TYPE_SMALL,
                 ColorA(1.0, 1.0, 1.0, 0.80), false);

    scoreVal = new WTextLabel();
    scoreVal->setup(mConfig, Vec2f(78,758), "n/a", FONT_TYPE_SMALL,
                 ColorA(0.9, 0.7, 0.9, 0.80), false);

    author = new WTextLabel();
    author->setup(mConfig, Vec2f(980,758), "vae.fi", FONT_TYPE_SMALL,
                    ColorA(0.7, 0.5, 0.7, 0.80), false);

    active = true;
}

void Mainmenu::draw() {
    title->draw();
    buttonStart->draw();
    buttonQuit->draw();
    score->draw();
    scoreVal->draw();
    author->draw();
}

void Mainmenu::update() { }

void Mainmenu::shutdown() {
    delete title;
    delete buttonStart;
    delete buttonQuit;
    delete score;
}

void Mainmenu::setPaused ( bool paused ) {
    if (paused == true) { buttonStart->setValue("resume", true); }
    else { buttonStart->setValue("new game", true); }
}

void Mainmenu::updateScore ( ) {
    scoreVal->setValue(toString(mConfig->hiScores->getScore()), false);
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
