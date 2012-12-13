//
//  OverlayFx.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/12/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"

#define VFX_EXPLOSION 0

class OverlayFxManager;

class OverlayFx {
public:
	void setup( Configuration *config, OverlayFxManager *master, ColorA newcolor,
               Vec2f newpos, int newtype );
	void update();
	void draw();
	void shutdown();
private:
    inline void drawExplosion();
    
    Configuration *mConfig;
    OverlayFxManager *mMaster;

    bool finished;

    ColorA color;
    double size;
    
    Vec2f pos;
    
    int type;
};
