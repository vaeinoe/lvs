//
//  Mainmenu.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 11/19/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Common.h"
#include "Configuration.h"

class Mainmenu {
public:
	void setup(Configuration *config);
	void update();
	void draw();
	void shutdown();
    void mouseMove ( const MouseEvent event );
    void mouseDown ( const MouseEvent event );
	void guiEvent( ciUIEvent *event );
private:
    ciUICanvas *gui;
    Vec2i *mMouseLoc;
    Configuration *mConfig;
};
