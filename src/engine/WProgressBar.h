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
	void setup(ColorA color, int minval, int maxval, int initval);
	void update();
    void reset();
	void draw();
	void shutdown();
    void setValue(int newVal);
    void setColor(ColorA color);
private:
    ColorA barColor;

    Vec2f pos;
    Vec2f size;
    
    int minval;
    int maxval;
    
    int value;
};
