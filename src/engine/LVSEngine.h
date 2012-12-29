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
#include "FadeObserver.h"
#include "Resources.h"

class Toolbar;
class World;
class Player;
class Audio;
class Configuration;
class Mainmenu;
class FaderPack;
class Fader;
class OverlayFxManager;

class LVSEngine : public FadeObserver {
public:
	void setup(Configuration *config);
	void update();
	void draw();
	void shutdown();
    void mouseMove ( const MouseEvent event );
    void mouseDown ( const MouseEvent event );
    void mouseUp   ( const MouseEvent event );
    void mouseDrag ( const MouseEvent event );
    
    void keyDown ( const KeyEvent event );

    void loadAll();
    void quitGame();
    void startGame();
    void backToMain();
    
    void setPlayfield();
    
    void addGameTime(int seconds);
    double getGameTime();
    double getMaxTime();
    
    void onFadeEnd(int typeId);
    
    void addCirclePoly( const Vec2f &center, const float radius,
                        const int numSegments, const ColorA &color );
    bool dragging;
    inline bool isVictory();

private:
    inline bool checkVictory();
    
    inline void drawGame(float lightness);
    inline void drawQueue();
    inline void gameOver();
    inline void resetGame();
    
    void fade(int dir, double seconds, bool quitAfter);
    inline void performFade();
    
    int gameState;
    bool fullScreen;

    int loadState;
    string loadStr;
    
    Fader *gameFader;
    bool paused;
    
    Fader *screenFader;
    double fadeVal;
    bool fading;
    
    Mainmenu *mMenu;
    Toolbar *mToolbar;
    World *mWorld;
    Player *mPlayer;
    Audio *mAudio;
    FaderPack *mFaders;
    OverlayFxManager *overlayFx;

    Configuration *mConfig;

    Vec2i *mMouseLoc;

    vector< GLfloat > tileVerts;
    vector< GLfloat > tileColors;
    
    void precalc();
    vector< Vec2f > precalcAngles[NUM_ANGLES + 1];
};
