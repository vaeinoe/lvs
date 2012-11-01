/*
 *  Toolbar.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "Common.h"

class Toolbar {
public:
	void setup( const Vec2i loc, const Vec2i size );
	void update( int fftDataBins );
	void draw();
	void shutdown();
	void guiEvent( ciUIEvent *event );
private:
	ciUICanvas *gui;
    ciUILabel *freqLabel;
};
