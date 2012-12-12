//
//  OverlayFxManager.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/12/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"
#include "Common.h"
#include "OverlayFx.h"

class OverlayFx;

class OverlayFxManager {
public:
	void setup( Configuration *config );
	void update();
	void draw();
	void shutdown();
    
    void createExplosion(int type, Vec2f pos, ColorA color, int fadeTime);
    void createText(int type, Vec2f pos, ColorA color, int fadeTime);
private:
    Configuration *mConfig;
    vector<OverlayFx*> fxList;
};
