//
//  OverlayFxManager.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/12/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "OverlayFxManager.h"
#include "OverlayFx.h"


void OverlayFxManager::setup( Configuration *config )
{
    mConfig = config;
}

inline bool OverlayFxManager::isFinished (OverlayFx *fx) { return fx->finished; }

void OverlayFxManager::update()
{
    for( vector<OverlayFx*>::iterator t = fxList.begin(); t != fxList.end(); ++t ){
        (*t)->update();
    }
    
    fxList.erase(remove_if(fxList.begin(), fxList.end(), isFinished), fxList.end());
//    cout << fxList.size() << endl;
}

void OverlayFxManager::drawDelayed()
{
    for( vector<OverlayFx*>::iterator t = fxList.begin(); t != fxList.end(); ++t ){
        if ((*t)->type == VFX_EXPLOSION) {
            (*t)->draw();
        }
    }
}

void OverlayFxManager::drawImmediate()
{
    for( vector<OverlayFx*>::iterator t = fxList.begin(); t != fxList.end(); ++t ){
        if ((*t)->type == VFX_TEXT) {
            (*t)->draw();            
        }
    }
}

void OverlayFxManager::shutdown()
{
    for( vector<OverlayFx*>::iterator t = fxList.begin(); t != fxList.end(); ++t ){
        (*t)->shutdown();
    }
    fxList.clear();
}

void OverlayFxManager::createExplosion(Vec2f pos, ColorA color)
{
    OverlayFx *fx = new OverlayFx();
    fx->setupExplosion(mConfig, this, color, pos);
    fxList.push_back(fx);
}

void OverlayFxManager::createText(Vec2f pos, ColorA color, int size, string text)
{
    OverlayFx *fx = new OverlayFx();
    fx->setupText(mConfig, this, color, pos, size, text);
    fxList.push_back(fx);
}
