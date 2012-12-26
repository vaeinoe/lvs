//
//  WTextLabel.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/26/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "WTextLabel.h"

void WTextLabel::setup(Configuration *config, Vec2f initpos, string newText,
                       int newType, ColorA newColor, bool centered)
{
    mConfig = config;
    pos     = initpos;
    type    = newType;
    color   = newColor;
    drawBox = false;
    
    setValue(newText);
    
    if (centered) { alignCenter(); }
}

inline void WTextLabel::alignCenter()
{
    Vec2f size = getSize();
    pos.x = mConfig->fieldSize.x / 2 - (size.x / 2);
    setValue(text);
}

void WTextLabel::draw()
{
    gl::color(color);
    switch (type) {
        case FONT_TYPE_SMALL:
            mConfig->fontSmall->drawString(text, mConfig->fieldOrigin + pos);
            break;            
        case FONT_TYPE_MEDIUM:
            mConfig->fontMedium->drawString(text, mConfig->fieldOrigin + pos);
            break;            
        case FONT_TYPE_LARGE:
            mConfig->fontLarge->drawString(text, mConfig->fieldOrigin + pos);
            break;
    }
    
    if (drawBox) {
        gl::drawStrokedRect(boundingBox + mConfig->fieldOrigin);        
    }
}

inline Vec2f WTextLabel::getSize()
{
    switch (type) {
        case FONT_TYPE_SMALL:
            return mConfig->fontSmall->measureString(text);
            break;
        case FONT_TYPE_MEDIUM:
            return mConfig->fontMedium->measureString(text);
            break;
        case FONT_TYPE_LARGE:
            return mConfig->fontLarge->measureString(text);
            break;
    }
    
    return Vec2f(0,0);
}

void WTextLabel::shutdown() { }

void WTextLabel::setValue(string newText)
{
    text = newText;
    
    Vec2f size = getSize();
    Vec2f top = Vec2f(pos.x, pos.y - (0.8 * size.y));
    
    boundingBox = Rectf(top, top + size);
}

void WTextLabel::setColor(ColorA newColor)
{
    color = newColor;
}

bool WTextLabel::isHit(Vec2f pos)
{
    return (boundingBox + mConfig->fieldOrigin).contains(pos);
}
