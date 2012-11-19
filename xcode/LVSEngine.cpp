/*
 *  LVSEngine.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

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

    mAudio->setup(config);
    mToolbar->setup(config, Vec2i(0, 0), Vec2i(getWindowWidth(), config->toolbarHeight));
    mWorld->setup(config, Vec2i(config->worldWidth, config->worldHeight));
    mPlayer->setup(config);
    mMenu->setup(config);

    mMouseLoc = new Vec2i(0, 0);
    
    gameRunning = false;
}

void LVSEngine::update()
{
    mAudio->update();
    if (gameRunning) {
        mToolbar->update(mAudio->dataSize);
        mWorld->update(mMouseLoc, mAudio->freqData, mAudio->dataSize);
        mPlayer->update();
    }
    else {
        mMenu->update();
    }
}

void LVSEngine::draw()
{
    float lightness = sin(getElapsedSeconds() / 10);
    if (gameRunning) {
        gl::clear( Color( 0, 0, lightness * 0.4 ) );
        mWorld->draw();
        mToolbar->draw();
        mPlayer->draw();
    }
    else {
        gl::clear( Color( lightness * 0.4, 0, 0.2 ) );
        mAudio->draw();
        mMenu->draw();
    }
}

void LVSEngine::mouseMove ( const MouseEvent event ) {
    Vec2i pos = event.getPos();
    mMouseLoc->x = pos.x;
    mMouseLoc->y = pos.y;
}

void LVSEngine::mouseDown ( const MouseEvent event ) {
    Vec2i pos = event.getPos();
    if (gameRunning) {
        if ( event.isLeft() ) {
            mWorld->selectTile(pos);
        }        
    }
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
