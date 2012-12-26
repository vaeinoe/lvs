//
//  WTextLabel.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/26/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"

class WTextLabel {
public:
	void setup(Configuration *config, Vec2f initpos, string newText,
               int newType, ColorA newColor, bool centered = false);
	void draw();
	void shutdown();
    void setValue(string newText);
    void setColor(ColorA newColor);
    
    bool isHit(Vec2f pos);
    
    friend class WTextButton;
private:
    inline void alignCenter();
    inline Vec2f getSize();
    Configuration *mConfig;
    
    int type;
    Vec2f pos;
    string text;
    ColorA color;
    
    Rectf boundingBox;
    bool drawBox;
};
