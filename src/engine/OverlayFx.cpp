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

#define ALPHA_TRESHOLD 0.03

void OverlayFx::setupExplosion( Configuration *config, OverlayFxManager *master,
                                ColorA newcolor, Vec2f newpos)
{
    mConfig = config;
    type = VFX_EXPLOSION;

    pos = newpos;
    size = 1;
    color = newcolor;

    finished = false;
}

void OverlayFx::setupText( Configuration *config, OverlayFxManager *master, ColorA newcolor,
                           Vec2f newpos, int newSize, string newText, bool centered )
{
    mConfig = config;
    type = VFX_TEXT;

    pos = newpos;
    textSize = newSize;
    color = newcolor;
    
    text = newText;
    
    finished = false;
    if (centered) centerText();
}

inline void OverlayFx::centerText()
{
    Vec2f boxSize;
    
    switch (textSize) {
        case FONT_TYPE_SMALL:
            boxSize = mConfig->fontSmall->measureString(text);
            break;
        case FONT_TYPE_MEDIUM:
            boxSize = mConfig->fontMedium->measureString(text);
            break;
        case FONT_TYPE_LARGE:
            boxSize = mConfig->fontLarge->measureString(text);
            break;
    }

    pos.x = mConfig->fieldSize.x / 2 - (boxSize.x / 2);
    pos.y = mConfig->fieldSize.y / 2 - (boxSize.y / 2);
}


void OverlayFx::update() { finished = color.a < ALPHA_TRESHOLD; }

void OverlayFx::draw()
{
    if (!finished)
    {
        if (type == VFX_EXPLOSION) { drawExplosion(); }
        else if (type == VFX_TEXT) { drawText(); }
    }
}

inline void OverlayFx::drawText()
{
    gl::color(color);
    switch (textSize) {
        case FONT_TYPE_SMALL:
            mConfig->fontSmall->drawString(text, mConfig->fieldOrigin + pos);
            gl::color(color.r, color.g, color.b, color.a * 0.25);
            mConfig->fontSmall->drawString(text, mConfig->fieldOrigin + pos + Vec2f(2,2));
            break;
        case FONT_TYPE_MEDIUM:
            mConfig->fontMedium->drawString(text, mConfig->fieldOrigin + pos);
            gl::color(color.r, color.g, color.b, color.a * 0.25);
            mConfig->fontMedium->drawString(text, mConfig->fieldOrigin + pos + Vec2f(2,2));
            break;
        case FONT_TYPE_LARGE:
            mConfig->fontLarge->drawString(text, mConfig->fieldOrigin + pos);
            gl::color(color.r, color.g, color.b, color.a * 0.25);
            mConfig->fontLarge->drawString(text, mConfig->fieldOrigin + pos + Vec2f(2,2));
            break;
    }
    
    color.a = color.a * 0.968;
}

inline void OverlayFx::drawExplosion()
{
    for (int i = 1; i < 6; i++) {
        double factor = 1 / (i * 1.0);
        mConfig->engine->addCirclePoly( mConfig->fieldOrigin + pos, size * factor, 32, ColorA(color.r, color.g, color.b, color.a * factor) );
    }
    color.a = color.a * 0.968;
    size  = size * 1.075;
}

void OverlayFx::shutdown()
{
    
}
