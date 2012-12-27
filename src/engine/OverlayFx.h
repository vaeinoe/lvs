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
#define VFX_TEXT 1

class OverlayFxManager;

class OverlayFx {
public:
	void setupExplosion( Configuration *config, OverlayFxManager *master, ColorA newcolor,
               Vec2f newpos );
    void setupText( Configuration *config, OverlayFxManager *master, ColorA newcolor,
                   Vec2f newpos, int newSize, string newText, bool centered = true );

	void update();
	void draw();
	void shutdown();
    
    friend class OverlayFxManager;
private:
    inline void drawExplosion();
    inline void drawText();
    inline void centerText();
    
    Configuration *mConfig;
    OverlayFxManager *mMaster;

    bool finished;

    ColorA color;
    double size;
    int textSize;
    
    Vec2f pos;
    
    int type;
    string text;
};
