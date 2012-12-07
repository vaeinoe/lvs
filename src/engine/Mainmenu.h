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
    
    void activate();
    void deactivate();
private:
    bool active;
    ci::CallbackId callback;
    ciUICanvas *gui;
    Vec2i *mMouseLoc;
    Configuration *mConfig;
};
