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

#define MENU_OFFSET 140

void Mainmenu::setup(Configuration *config)
{
    mConfig = config;
    
    title = new WTextLabel();
    title->setup(mConfig, Vec2f(16,1 + MENU_OFFSET), GAME_NAME, FONT_TYPE_LARGE,
                 ColorA(0.7, 0.5, 0.7, 0.80), true);

    buttonStart = new WTextButton();
    buttonStart->setup(mConfig, Vec2f(16,47 + MENU_OFFSET), "new game", FONT_TYPE_MEDIUM,
                       ColorA(0.8, 0.5, 0.8, 0.66), true);

    buttonHow = new WTextButton();
    buttonHow->setup(mConfig, Vec2f(16,85 + MENU_OFFSET), "how to play", FONT_TYPE_MEDIUM,
                      ColorA(0.8, 0.5, 0.8, 0.66), true);

    buttonQuit = new WTextButton();
    buttonQuit->setup(mConfig, Vec2f(16,123 + MENU_OFFSET), "quit", FONT_TYPE_MEDIUM,
                      ColorA(0.8, 0.5, 0.8, 0.66), true);

    score = new WTextLabel();
    score->setup(mConfig, Vec2f(8,758), "best time:", FONT_TYPE_SMALL,
                 ColorA(1.0, 1.0, 1.0, 0.80), false);

    scoreVal = new WTextLabel();
    scoreVal->setup(mConfig, Vec2f(78,758), "n/a", FONT_TYPE_SMALL,
                 ColorA(0.9, 0.7, 0.9, 0.80), false);

    author = new WTextLabel();
    author->setup(mConfig, Vec2f(940,758), toString("v") + toString(VERSION_STR) + toString("   vae.fi"), FONT_TYPE_SMALL,
                    ColorA(0.7, 0.5, 0.7, 0.80), false);

    active = true;
}

void Mainmenu::draw() {
    title->draw();
    buttonStart->draw();
    buttonHow->draw();
    buttonQuit->draw();
    score->draw();
    scoreVal->draw();
    author->draw();
}

void Mainmenu::update() { }

void Mainmenu::shutdown() {
    delete title;
    delete buttonStart;
    delete buttonHow;
    delete buttonQuit;
    delete score;
    delete scoreVal;
    delete author;
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
        buttonHow->mousePos(pos);
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
        else if (buttonHow->isHit(pos)) {
            mConfig->engine->showInstructions();
        }
    }
}

void Mainmenu::activate() { active = true; }
void Mainmenu::deactivate() { active = false; }
