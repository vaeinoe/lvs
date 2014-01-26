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
class Lvs;

class LVSEngine : public FadeObserver {
public:
	void setup(Lvs *lvsApp, Configuration *config);
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
    
    void bufferStrokedCircle( const Vec2f &center, const float radius,
                        const int numSegments, const ColorA &color );
    void bufferSolidCircle( const Vec2f &center, const float radius,
                            int numSegments, const ColorA &color );
    void bufferLine( const Vec2f start, const Vec2f end,
                     const ColorA colorStart, const ColorA colorEnd );
    
    bool dragging;
    bool isVictory();

private:
	Lvs *mApp;

    inline bool checkVictory();
    inline void checkTime();
    
    inline void drawGame(float lightness);
    inline void drawBuffer();
    inline void gameOver();
    inline void resetGame();
    
    void fade(int dir, double seconds, bool quitAfter);
    inline void performFade();
    
    double endGameTime;
    
    int gameState;
    bool timeRunningOut;
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
    AudioEngine *mAudio;
    FaderPack *mFaders;
    OverlayFxManager *overlayFx;
    HighScores *mHiScores;
    
    Configuration *mConfig;

    Vec2i *mMouseLoc;

    vector< GLfloat > lineVerts;
    vector< GLfloat > lineColors;
    
    vector< GLfloat > triVerts;
    vector< GLfloat > triColors;
    
    void precalc();
    vector< Vec2f > precalcAngles[NUM_ANGLES + 1];
};
