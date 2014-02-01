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
class WTextLabel;
class WTextButton;

class Mainmenu {
public:
	void setup(Configuration *config);
	void update();
	void draw();
	void shutdown();
    void mouseMove ( const MouseEvent event );
    void mouseDown ( const MouseEvent event );
	void guiEvent( ciUIEvent *event );
    
    void setPaused(bool paused);
    
    void activate();
    void deactivate();
    void updateScore();
private:
    bool active;
    ciUICanvas *gui;
    Vec2i *mMouseLoc;
    Configuration *mConfig;
    
    WTextLabel *title;
    WTextButton *buttonStart;
    WTextButton *buttonHow;
    WTextButton *buttonQuit;
    WTextLabel *score;
    WTextLabel *scoreVal;

    WTextLabel *author;
};
