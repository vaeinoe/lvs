/*
 *  LVSEngine.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */
#pragma once

#include "Common.h"
#include "World.h"
#include "Toolbar.h"
#include "Player.h"
#include "Audio.h"

class LVSEngine {
public:
	void setup(Configuration *config);
	void update();
	void draw();
	void shutdown();
    void mouseMove ( const MouseEvent event );
    void mouseDown ( const MouseEvent event );
private:
    Toolbar *mToolbar;
    World *mWorld;
    Player *mPlayer;
    Vec2i *mMouseLoc;
    Audio *mAudio;
    Configuration *mConfig;
};
