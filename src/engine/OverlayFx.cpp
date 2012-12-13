//
//  OverlayFx.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/12/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "OverlayFx.h"
#include "LVSEngine.h"
#include "OverlayFxManager.h"

#define ALPHA_TRESHOLD 0.1

void OverlayFx::setup( Configuration *config, OverlayFxManager *master, ColorA newcolor,
                      Vec2f newpos, int newtype )
{
    size = 1;
    color = newcolor;
    
    mConfig = config;

    finished = false;
    type = newtype;
    pos = newpos;
}

void OverlayFx::update()
{
    finished = color.a < ALPHA_TRESHOLD;
}

void OverlayFx::draw()
{
    if (!finished)
    {
        if (type == VFX_EXPLOSION) { drawExplosion(); }
    }
}

inline void OverlayFx::drawExplosion()
{
    for (int i = 1; i < 6; i++) {
        double factor = 1 / (i * 1.0);
        mConfig->engine->addCirclePoly( pos, size * factor, 32, ColorA(color.r, color.g, color.b, color.a * factor) );
    }
    color.a = color.a * 0.968;
    size  = size * 1.075;
}

void OverlayFx::shutdown()
{
    
}
