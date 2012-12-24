//
//  WProgressBar.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/15/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "WProgressBar.h"

void WProgressBar::setup(Vec2f initpos, Vec2f initsize, ColorA color,
                         int newMaxval, int initval)
{
    pos  = initpos;
    size = initsize;
    
    barColor = color;
    maxval   = newMaxval;
    value    = initval;
}

void WProgressBar::draw()
{
    double progress = value * 1.0 / maxval;
    Vec2f padding = Vec2f(3,3);
    
    Vec2f start = pos + padding;
    Vec2f end = pos + Vec2f((size.x - padding.x) * progress, (size.y - padding.y));
    if (end.x > start.x) {
        gl::color(barColor);
        gl::drawSolidRect( Rectf(start, end) );
    }
    
    gl::color( 1.0, 1.0, 1.0, 0.25 );
    gl::drawStrokedRect( Rectf(pos, pos + size) );
}


void WProgressBar::setValue(int newVal, int newMax)
{
    value = newVal;
    if (newMax != -1) maxval = newMax;
}

void WProgressBar::setColor(ColorA color) { barColor = color; }
void WProgressBar::shutdown() { }
