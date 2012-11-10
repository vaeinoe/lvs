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

    mAudio->setup(config);
    mToolbar->setup(config, Vec2i(0, 0), Vec2i(getWindowWidth(), config->toolbarHeight));
    mWorld->setup(config, Vec2i(config->worldWidth, config->worldHeight));
    //    mPlayer->setup();

    mMouseLoc = new Vec2i(0, 0);
}

void LVSEngine::update()
{
    mAudio->update();
    mToolbar->update(mAudio->dataSize);
    mWorld->update(mMouseLoc, mAudio->freqData, mAudio->dataSize);
    //    mPlayer->update();
}

void LVSEngine::draw()
{
    mAudio->draw();
    mWorld->draw();
    mToolbar->draw();
    //    mPlayer->draw();
}

void LVSEngine::mouseMove ( const MouseEvent event ) {
    Vec2i pos = event.getPos();
    mMouseLoc->x = pos.x;
    mMouseLoc->y = pos.y;
}

void LVSEngine::mouseDown ( const MouseEvent event ) {
    Vec2i pos = event.getPos();
    if ( event.isLeft() ) {
        mWorld->selectTile(pos);
    }
}

void LVSEngine::shutdown()
{
    mToolbar->shutdown();
    mWorld->shutdown();
    mAudio->shutdown();
    //    mPlayer->shutdown();
    
    delete mToolbar;
    delete mWorld;
    delete mPlayer;
    delete mAudio;
}
