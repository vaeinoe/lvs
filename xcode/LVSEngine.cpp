/*
 *  LVSEngine.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#define S_LOADING   0
#define S_MAINMENU  1
#define S_INGAME_1  2

#define RES_FONT "nevis.ttf"

#include "LVSEngine.h"

void LVSEngine::setup(Configuration *config)
{    
    mConfig = config;
    mAudio = new Audio();
    mToolbar = new Toolbar();
    mWorld = new World();
    mPlayer = new Player();
    mMenu = new Mainmenu();

    mConfig->engine = this;
    mConfig->toolbar = mToolbar;
    mConfig->world = mWorld;
    mConfig->player = mPlayer;

    mMouseLoc = new Vec2i(0, 0);

    gameState = S_LOADING;
    
    loadState = 0;
    loadStr = "wait 5";

    loadFont = Font( loadResource( RES_FONT ), 32);
    texFont = gl::TextureFont::create(loadFont);
}

void LVSEngine::loadAll() {
    switch (loadState) {
        case 0:
            mToolbar->setup(mConfig, Vec2i(0, 0), Vec2i(getWindowWidth(), mConfig->toolbarHeight));
            loadStr = "wait 4";
            break;
        case 1:
            mWorld->setup(mConfig, Vec2i(mConfig->worldWidth, mConfig->worldHeight));
            loadStr = "wait 3";
            break;
        case 2:
            mPlayer->setup(mConfig);
            loadStr = "wait 2";
            break;
        case 3:
            mMenu->setup(mConfig);
            loadStr = "wait 1";
            break;
        case 4:
            mAudio->setup(mConfig);
            loadStr = "wait 0";
            break;
        case 5:
            cout << "Running." << endl;
            gameState = S_MAINMENU;
            break;
    }

    loadState++;
}

void LVSEngine::update()
{
    switch (gameState) {
        case S_LOADING:
            loadAll();
            break;
        case S_MAINMENU:
            mAudio->update();
            mMenu->update();
            break;
        case S_INGAME_1:
            mAudio->update();
            mToolbar->update(mAudio->dataSize);
            mWorld->update(mMouseLoc, mAudio->freqData, mAudio->dataSize);
            mPlayer->update();
            break;
    }    
}

void LVSEngine::draw()
{
    float lightness = sin(getElapsedSeconds() / 10);
    switch (gameState) {
        case S_LOADING:
            gl::clear( Color( 0, 0, 0 ) );
            texFont->drawString(loadStr,Vec2f(50,50));
//            gl::drawString(loadStr, Vec2f(50,50), Color(1,1,1), loadFont);
            break;
        case S_MAINMENU:
            gl::clear( Color( lightness * 0.4, 0, 0.2 ) );
            mAudio->draw();
            mMenu->draw();
            break;
        case S_INGAME_1:
            gl::clear( Color( 0, 0, lightness * 0.4 ) );
            mWorld->draw();
            mToolbar->draw();
            mPlayer->draw();
            break;
    }
}

void LVSEngine::mouseMove ( const MouseEvent event ) {
    Vec2i pos = event.getPos();
    mMouseLoc->x = pos.x;
    mMouseLoc->y = pos.y;
}

void LVSEngine::mouseDown ( const MouseEvent event ) {
    Vec2i pos = event.getPos();
    switch (gameState) {
            case S_INGAME_1:
            if ( event.isLeft() ) {
                mWorld->selectTile(pos);
            }
            break;
    }
}

void LVSEngine::keyDown ( const KeyEvent event ) {
    if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
        switch (gameState) {
            case S_MAINMENU:
                quitGame();
                break;
            case S_INGAME_1:
                quitGame();
                break;
        }
    }
    if( event.getCode() == KeyEvent::KEY_r ) {
        if (gameState == S_INGAME_1) {
            mWorld->reset();
        }
    }
}

void LVSEngine::startGame()
{
    // TODO: fade in
    gameState = S_INGAME_1;
}

void LVSEngine::quitGame()
{
    // TODO: fade out
    shutdown();
    exit(0);
}

void LVSEngine::shutdown()
{
    mToolbar->shutdown();
    mWorld->shutdown();
    mAudio->shutdown();
    mPlayer->shutdown();
    mMenu->shutdown();
    
    delete mToolbar;
    delete mWorld;
    delete mPlayer;
    delete mAudio;
    delete mMenu;
}
