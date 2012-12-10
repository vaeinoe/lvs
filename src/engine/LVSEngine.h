/*
 *  LVSEngine.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */
#pragma once
#include "Configuration.h"
#include "Common.h"
#include "World.h"
#include "Toolbar.h"
#include "Player.h"
#include "Audio.h"
#include "Mainmenu.h"
#include "Resources.h"
#include "FaderPack.h"
#include "Fader.h"
#include "FadeObserver.h"

class Toolbar;
class World;
class Player;
class Audio;
class Configuration;
class Mainmenu;
class FaderPack;
class Fader;

class LVSEngine : public FadeObserver {
public:
	void setup(Configuration *config);
	void update();
	void draw();
	void shutdown();
    void mouseMove ( const MouseEvent event );
    void mouseDown ( const MouseEvent event );
    void keyDown ( const KeyEvent event );

    void loadAll();
    void quitGame();
    void startGame();
    void backToMain();
    
    void onFadeEnd(int typeId);
private:
    void fade(int dir, double seconds, bool quitAfter);
    inline void performFade();
    
    int gameState;
    bool fullScreen;

    Font loadFont;
    gl::TextureFontRef texFont;
    
    int loadState;
    string loadStr;
    
    Fader *screenFader;
    double fadeVal;
    bool fading;
    bool quitAfterFade;
    
    Mainmenu *mMenu;
    Toolbar *mToolbar;
    World *mWorld;
    Player *mPlayer;
    Vec2i *mMouseLoc;
    Audio *mAudio;
    FaderPack *mFaders;

    Configuration *mConfig;
};
