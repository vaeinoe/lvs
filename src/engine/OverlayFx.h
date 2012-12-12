//
//  OverlayFx.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/12/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"
#include "Common.h"
#include "OverlayFxManager.h"

class OverlayFxManager;

class OverlayFx {
public:
	void setup( Configuration *config, OverlayFxManager *master );
	void update();
	void draw();
	void shutdown();
private:
    Configuration *mConfig;
    OverlayFxManager *mMaster;
    bool finished;
};
