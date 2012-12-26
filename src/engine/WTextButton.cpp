//
//  WTextButton.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/26/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "WTextButton.h"

void WTextButton::setup(Configuration *config, Vec2f initpos, string newText,
                        int newType, ColorA newColor, bool centered)
{
    label = new WTextLabel();
    label->setup(config, initpos, newText, newType, newColor, centered);
    // label->drawBox = true;
    
    colorNormal = newColor;
    colorHover  = ColorA( 1.0, 1.0, 1.0, 1.0 );
}

void WTextButton::draw()
{
    label->draw();
}

void WTextButton::mousePos(Vec2f pos)
{
    if (label->isHit(pos)) {
        label->setColor(colorHover);
    }
    else {
        label->setColor(colorNormal);
    }
}

bool WTextButton::isHit(Vec2f pos)
{
    return label->isHit(pos);
}

void WTextButton::shutdown() { delete label; }

void WTextButton::setValue(string newText)
{
    label->setValue(newText);
}
