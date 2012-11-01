/*
 *  Player.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */
#pragma once
#include "Common.h"

class Player {
public:
	void setup();
	void update( const Vec2i pos );
	void draw();
	void shutdown();
};
