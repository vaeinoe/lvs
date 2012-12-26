//
//  WTextLabel.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/26/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"
#include "WTextLabel.h"

class WTextButton {
public:
	void setup(Configuration *config, Vec2f initpos, string newText,
               int newType, ColorA newColor, bool centered = false);
	void draw();
	void shutdown();
    void setValue(string newText);
    
    void mousePos(Vec2f pos);
    bool isHit(Vec2f pos);
private:
    ColorA colorNormal;
    ColorA colorHover;
    WTextLabel *label;
};
