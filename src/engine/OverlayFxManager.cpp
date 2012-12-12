//
//  OverlayFxManager.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/12/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "OverlayFxManager.h"


void OverlayFxManager::setup( Configuration *config )
{
    mConfig = config;
}

void OverlayFxManager::update()
{
    for( vector<OverlayFx*>::iterator t = fxList.begin(); t != fxList.end(); ++t ){
        (*t)->update();
    }
    
    // XXX: delete all finished
}

void OverlayFxManager::draw()
{
    for( vector<OverlayFx*>::iterator t = fxList.begin(); t != fxList.end(); ++t ){
        (*t)->draw();
    }
}

void OverlayFxManager::shutdown()
{
    for( vector<OverlayFx*>::iterator t = fxList.begin(); t != fxList.end(); ++t ){
        (*t)->shutdown();
    }
    fxList.clear();
}

void OverlayFxManager::createExplosion(int type, Vec2f pos, ColorA color, int fadeTime)
{
    
}

void OverlayFxManager::createText(int type, Vec2f pos, ColorA color, int fadeTime)
{
    
}
