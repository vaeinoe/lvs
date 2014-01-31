//
//  OverlayFxManager.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/12/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"

class OverlayFx;

class OverlayFxManager {
public:
	void setup( Configuration *config );
	void update();
	void drawDelayed();
    void drawImmediate();
	void shutdown();
    
    void createExplosion(Vec2f pos, ColorA color);
    void createText(Vec2f pos, ColorA color, int size, string text);
private:
    static inline bool isFinished (OverlayFx *fx);
    
    Configuration *mConfig;
    vector<OverlayFx*> fxList;
};
