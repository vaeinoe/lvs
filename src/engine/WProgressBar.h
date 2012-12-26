//
//  WProgressBar.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/15/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"

class WProgressBar {
public:
	void setup(Configuration *config, Vec2f initpos, Vec2f initsize, ColorA color,
               int newMaxval, int initval);
	void draw();
	void shutdown();
    void setValue(int newVal, int newMax = -1);
    void setColor(ColorA color);
private:
    Configuration *mConfig;
    ColorA barColor;

    Vec2f pos;
    Vec2f size;
    
    int maxval;
    int value;
};
