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
#define S_QUITTING  3

#include "LVSEngine.h"

void LVSEngine::setup(Configuration *config)
{        
    fadeTimer   = new Timer();
    fadeDir     = 0;
    fadeTimeSec = 0;
    quitAfterFade = false;

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
    
    fullScreen = false;
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
            fade(1, 2.5, false);
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
            mToolbar->update(mAudio->getDataSize());
            mWorld->update(mMouseLoc, mAudio->getFreqData(), mAudio->getDataSize());
            mPlayer->update();
            break;
        case S_QUITTING:
            mAudio->update();
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
        case S_QUITTING:
            mAudio->draw();
            mMenu->draw();
            break;
    }
    if (fadeDir != 0) { performFade(); }
}

// Fades out and eventually quits game
inline void LVSEngine::performFade() {
    if (fadeDir != 0 && !fadeTimer->isStopped()) {
        double time = fadeTimer->getSeconds();
        if (time > fadeTimeSec) {
            fadeDir = 0;
            fadeTimer->stop();
            if (quitAfterFade) {
                gl::clear( Color( 0, 0, 0 ) );
                shutdown();
                exit(0);
            }
        }
        else {
            double fadePoint = time / fadeTimeSec;
            if (fadeDir < 0) { gl::color(0, 0, 0, fadePoint); }
            else { gl::color(0, 0, 0, 1 - fadePoint); }
            gl::drawSolidRect (Rectf(0,0,getWindowWidth(),getWindowHeight()));
        }
    }    
}

// Fades in or out
void LVSEngine::fade(int dir, double seconds, bool quitAfter) {
    fadeDir = dir;
    fadeTimeSec = seconds;
    quitAfterFade = quitAfter;
    fadeTimer->start();
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
                // TODO: reset player data add a return button to main?
                backToMain();
                break;
        }
    }
    if( event.getCode() == KeyEvent::KEY_r ) {
        if (gameState == S_INGAME_1) { mWorld->reset(); }
    }
    
    if ( event.getCode() == KeyEvent::KEY_f ) {
        fullScreen = !fullScreen;
        setFullScreen(fullScreen);
    }
}

void LVSEngine::startGame()
{
    gameState = S_INGAME_1;
    mMenu->deactivate();
    mAudio->fadeToPreset(2, 5.0);
}

void LVSEngine::backToMain()
{
    gameState = S_MAINMENU;
    mMenu->activate();
    mAudio->fadeToPreset(1, 5.0);
}

void LVSEngine::quitGame()
{
    gameState = S_QUITTING;
    mMenu->deactivate();
    mAudio->fadeToPreset(0, 2.5);
    fade(-1, 3.0, true);
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
