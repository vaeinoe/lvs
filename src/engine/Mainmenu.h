//
//  Mainmenu.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 11/19/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"

class ciUICanvas;
class ciUIEvent;

class Mainmenu {
public:
	void setup(Configuration *config);
	void update();
	void draw();
	void shutdown();
    void mouseMove ( const MouseEvent event );
    void mouseDown ( const MouseEvent event );
	void guiEvent( ciUIEvent *event );
    
    void activate();
    void deactivate();
private:
    bool active;
    ciUICanvas *gui;
    svg::Doc *logo;
    Vec2i *mMouseLoc;
    Configuration *mConfig;
};
