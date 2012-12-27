/*
 *  LVSEngine.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#define S_LOADING   0
#define S_MAINMENU  1
#define S_INGAME_1  2
#define S_GAMEOVER  3
#define S_VICTORY   4
#define S_QUITTING  5

#define FADEFADER 0
#define GAMEFADER 1

#include "LVSEngine.h"
#include "World.h"
#include "Toolbar.h"
#include "Player.h"
#include "Audio.h"
#include "Mainmenu.h"
#include "FaderPack.h"
#include "Fader.h"
#include "OverlayFxManager.h"
#include "TileLevel.h"

void LVSEngine::setup(Configuration *config)
{
    fadeVal = 1.0;

    mConfig = config;
    mAudio = new Audio();
    mToolbar = new Toolbar();
    mWorld = new World();
    mPlayer = new Player();
    mMenu = new Mainmenu();
    mFaders = new FaderPack();
    overlayFx = new OverlayFxManager();

    mConfig->engine = this;
    mConfig->toolbar = mToolbar;
    mConfig->world = mWorld;
    mConfig->player = mPlayer;
    mConfig->faders = mFaders;
    mConfig->audio = mAudio;
    mConfig->overlayFx = overlayFx;

    setPlayfield();
    
    for (int i = 0; i < config->numTileTypes; i++) {
        mConfig->levels[i] = new TileLevel();
        mConfig->levels[i]->setup(mConfig, i);
    }

    dragging = false;

    mMouseLoc = new Vec2i(0, 0);

    gameState = S_LOADING;
    paused = false;

    Font loadFont = Font( loadResource( RES_FONT ), FONT_SIZE_SMALL);
    mConfig->fontSmall = gl::TextureFont::create(loadFont);
    loadFont = Font( loadResource( RES_FONT ), FONT_SIZE_MEDIUM);
    mConfig->fontMedium = gl::TextureFont::create(loadFont);
    loadFont = Font( loadResource( RES_FONT ), FONT_SIZE_LARGE);
    mConfig->fontLarge = gl::TextureFont::create(loadFont);

    loadState = 0;
    loadStr = "mixing cupcake batter";
    
    fullScreen = false;

    mFaders->setup(mConfig, true);
    screenFader = mFaders->createFader(FADEFADER, &fadeVal, this);

    gameFader = mFaders->createFader(GAMEFADER);
    gameFader->addObserver(this);
}

void LVSEngine::loadAll() {
    switch (loadState) {
        case 0:
            mToolbar->setup(mConfig, Vec2i(0, 0), Vec2i(getWindowWidth(), mConfig->toolbarHeight));
            loadStr = "slicing the world into shards";
            break;
        case 1:
            precalc();
            overlayFx->setup(mConfig);
            mWorld->setup(mConfig, Vec2i(mConfig->worldWidth, mConfig->worldHeight));
            loadStr = "remembering your eyes";
            break;
        case 2:
            mPlayer->setup(mConfig);
            loadStr = "filling the glass half full";
            break;
        case 3:
            mMenu->setup(mConfig);
            loadStr = "preparing the picardy third";
            break;
        case 4:
            mAudio->setup(mConfig);
            loadStr = "ready for action";
            break;
        case 5:
            // cout << "Running." << endl;
            gameState = S_MAINMENU;
            
            screenFader->fade(0.0, 2.5);
            break;
    }

    loadState++;
}

void LVSEngine::update()
{
    checkVictory();
    mFaders->update();
    
    switch (gameState) {
        case S_LOADING:
            loadAll();
            break;
        case S_MAINMENU:
            mAudio->update();
            mMenu->update();
            break;
        case S_INGAME_1:
            mAudio->update();
            overlayFx->update();
            mToolbar->update(mAudio->getDataSize());
            mWorld->update(mMouseLoc, mAudio->getFreqData(), mAudio->getDataSize());
            mPlayer->update();
            break;
        case S_QUITTING:
            mAudio->update();
            break;
        default:
            mAudio->update();
            break;
    }
}

inline void LVSEngine::drawGame(float lightness)
{
    gl::color( 0, 0, lightness * 0.4 );
    gl::drawSolidRect(mConfig->fieldRect);

    double gameTime = getGameTime();
    if (gameTime < WARN_GAME_TIME && gameTime > 0.0) {
        double bri = fmod(gameTime, 1);
        double a   = (1 - (gameTime / WARN_GAME_TIME)) * 0.3;
        gl::color(bri, bri / 2, bri, a);
        gl::drawSolidRect(mConfig->fieldRect);
    }
    
    mWorld->draw();
    overlayFx->drawDelayed();
    drawQueue();
    mToolbar->draw();
    mPlayer->draw();
    overlayFx->drawImmediate();
}

void LVSEngine::draw()
{
    gl::clear( Color( 0.0, 0.0, 0.0 ) );
    
    float lightness = sin(getElapsedSeconds() / 10);
    switch (gameState) {
        case S_LOADING:
            gl::color(0, 0, 0.0, 1.0);
            gl::drawSolidRect(mConfig->fieldRect);
            gl::color(0.9, 0.9, 0.9, 0.5);
            mConfig->fontMedium->drawString(loadStr,getWindowCenter() - (mConfig->fontMedium->measureString(loadStr) / 2));
            break;
        case S_MAINMENU:
            gl::color( lightness * 0.4, 0, 0.2 );
            gl::drawSolidRect(mConfig->fieldRect);
            mAudio->draw();
            mMenu->draw();
            break;
        case S_INGAME_1:
            drawGame(lightness);
            break;
        case S_QUITTING:
            gl::color( lightness * 0.4, 0, 0.2 );
            gl::drawSolidRect(mConfig->fieldRect);
            mAudio->draw();
            mMenu->draw();
            break;
    }
    gl::color(1.0, 1.0, 1.0, 0.3);
    gl::drawStrokedRect (mConfig->fieldRect);

    if (screenFader->isActive()) {
        gl::color(0, 0, 0, fadeVal);
        gl::drawSolidRect (mConfig->fieldRect);
    }
    if (gameState == S_GAMEOVER) {
        gl::color(1, 1, 1, fadeVal);
        mConfig->fontLarge->drawString(ENDGAME_STR,getWindowCenter() - (mConfig->fontLarge->measureString(ENDGAME_STR) / 2));
    }
    else if (gameState == S_VICTORY) {
        gl::color(1, 1, 1, fadeVal);
        mConfig->fontMedium->drawString(WINGAME_STR,getWindowCenter() - (mConfig->fontMedium->measureString(WINGAME_STR) / 2));
    }
    
}

double LVSEngine::getGameTime() { return gameFader->timeLeft(); }
double LVSEngine::getMaxTime() { return gameFader->timeTotal(); }

void LVSEngine::mouseUp ( const MouseEvent event ) {
    Vec2i pos = event.getPos();
    switch (gameState) {
        case S_INGAME_1:
            if ( event.isLeft() ) { mWorld->deselectTile(pos); }
            break;
    }    
    dragging = false;
}

void LVSEngine::mouseDrag ( const MouseEvent event ) {
    Vec2i pos = event.getPos();
    mMouseLoc->x = pos.x;
    mMouseLoc->y = pos.y;    
}

void LVSEngine::mouseMove ( const MouseEvent event ) {
    Vec2i pos = event.getPos();
    mMouseLoc->x = pos.x;
    mMouseLoc->y = pos.y;
    switch (gameState) {
        case S_MAINMENU:
            mMenu->mouseMove(event);
            break;
    }
}

void LVSEngine::mouseDown ( const MouseEvent event ) {
    dragging = true;
    Vec2i pos = event.getPos();
    switch (gameState) {
        case S_INGAME_1:
            if ( event.isLeft() ) { mWorld->selectTile(pos); }
            break;
        case S_MAINMENU:
            mMenu->mouseDown(event);
            break;
    }
}

void LVSEngine::keyDown ( const KeyEvent event ) {
    if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
        switch (gameState) {
            case S_MAINMENU:
                quitGame();
                break;
            case S_INGAME_1:
                // TODO: add a return button to main?
                backToMain();
                break;
        }
    }

    // XXX implement proper support or remove
    if ( event.getCode() == KeyEvent::KEY_f ) {
        fullScreen = !fullScreen;
        setFullScreen(fullScreen);
        setPlayfield();
    }
}

inline bool LVSEngine::checkVictory()
{
    if (gameState == S_INGAME_1) {
        for (int i = 0; i < mConfig->numTileTypes; i++) {
            if (!mConfig->levels[i]->isFinished()) {
                return false;
            }
        }
        // OK, victory
        gameState = S_VICTORY;
        mAudio->fadeToPreset(1, 5.0);
        screenFader->fade(1.0, 3.0);
        return true;        
    }
    
    return false;
}

void LVSEngine::startGame()
{
    gameState = S_INGAME_1;
    mMenu->deactivate();
    mAudio->fadeToPreset(2, 5.0);
    
    if (paused) {
        // cout << "Resuming" << endl;
        gameFader->resume();
    }
    else {
        gameFader->fade(0.0, INIT_GAME_TIME);
    }
    paused = false;
}

void LVSEngine::backToMain()
{
    gameState = S_MAINMENU;
    mMenu->activate();
    mAudio->fadeToPreset(1, 5.0);
    
    gameFader->pause();
    paused = true;
}

void LVSEngine::quitGame()
{
    gameState = S_QUITTING;
    mMenu->deactivate();
    mAudio->fadeToPreset(0, 2.5);
    
    screenFader->fade(1.0, 3.0);
}

void LVSEngine::gameOver()
{
    gameState = S_GAMEOVER;
    mAudio->fadeToPreset(1, 5.0);
    screenFader->fade(1.0, 3.0);
}

inline void LVSEngine::resetGame()
{
    mWorld->reset();
    for (int i = 0; i < mConfig->numTileTypes; i++) {
        mConfig->levels[i]->reset();
    }
}

void LVSEngine::addGameTime(int seconds)
{
    gameFader->addTime(seconds);
}

void LVSEngine::onFadeEnd(int typeId)
{
    // Fade to black ended
    if (typeId == FADEFADER) {
        // Game over, reset score & back to main
        if (gameState == S_GAMEOVER) {
            resetGame();
            screenFader->fade(0.0, 2.0);
            gameState = S_MAINMENU;
            mMenu->activate();            
        }
        // Final fade, quit game
        else if (gameState == S_QUITTING) {
            gl::clear( Color( 0, 0, 0 ) );
            shutdown();
            exit(0);
        }
        else if (gameState == S_VICTORY) {
            resetGame();
            screenFader->fade(0.0, 2.0);
            gameState = S_MAINMENU;
            mMenu->activate();            
        }
    }
    // Game time ended
    else if (typeId == GAMEFADER) {
        gameOver();
    }
}

void LVSEngine::shutdown()
{
    mToolbar->shutdown();
    mWorld->shutdown();
    mAudio->shutdown();
    mPlayer->shutdown();
    mMenu->shutdown();
    mFaders->shutdown();
    
    delete mToolbar;
    delete mWorld;
    delete mPlayer;
    delete mAudio;
    delete mMenu;
    delete mFaders;
}

// Adds a polygon to rendering queue
void LVSEngine::addCirclePoly( const Vec2f &center, const float radius, int numSegments, const ColorA &color )
{
	// automatically determine the number of segments from the circumference
	if( numSegments <= 0 ) {
		numSegments = (int)math<double>::floor( radius * M_PI * 2 );
	}
	if( numSegments < 2 ) numSegments = 2;
    
    // first vertex
    GLfloat firstX = center.x + precalcAngles[numSegments][0].x * radius;
    GLfloat firstY = center.y + precalcAngles[numSegments][0].y * radius;
    tileVerts.push_back(firstX); tileVerts.push_back(firstY);
    tileColors.push_back(color.r); tileColors.push_back(color.g);
    tileColors.push_back(color.b); tileColors.push_back(color.a);
    
    // middle vertices
	for( int s = 1; s < numSegments; s++ ) {
        for (int i = 0; i < 2; i++) {
            tileVerts.push_back(center.x + precalcAngles[numSegments][s].x * radius);
            tileVerts.push_back(center.y + precalcAngles[numSegments][s].y * radius);
            tileColors.push_back(color.r); tileColors.push_back(color.g);
            tileColors.push_back(color.b); tileColors.push_back(color.a);
        }
	}
    
    // drawing line loops = back to first vertex in the end
    tileVerts.push_back(firstX); tileVerts.push_back(firstY);
    tileColors.push_back(color.r); tileColors.push_back(color.g);
    tileColors.push_back(color.b); tileColors.push_back(color.a);
}

// Draws the rendering line queue
inline void LVSEngine::drawQueue()
{
    glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
    
	glColorPointer( 4, GL_FLOAT, 0, &tileColors[0] );
	glVertexPointer( 2, GL_FLOAT, 0, &tileVerts[0] );
	glDrawArrays( GL_LINES, 0, tileVerts.size() / 2 );
    
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
    
    tileColors.clear();
    tileVerts.clear();
}

// Precalculate angle arrays for polygon drawing
inline void LVSEngine::precalc() {
    for (int i = 2; i < NUM_ANGLES + 1; i++) {
        for( int s = 0; s < i; s++ ) {
            float t = s / (float)i * 2.0f * 3.14159f;
            precalcAngles[i].push_back( Vec2f(math<float>::cos( t ), math<float>::sin( t )) );
        }
    }
}

// Set playfield parameters - called when switching between fullscreen and windowed
void LVSEngine::setPlayfield()
{
    mConfig->fieldSize = Vec2i(PLAYFIELD_W, PLAYFIELD_H);
    mConfig->fieldOrigin = getWindowCenter() - (Vec2f(PLAYFIELD_W, PLAYFIELD_H) / 2);
    mConfig->fieldRect = Rectf(mConfig->fieldOrigin.x, mConfig->fieldOrigin.y,
                               mConfig->fieldOrigin.x + mConfig->fieldSize.x,
                               mConfig->fieldOrigin.y + mConfig->fieldSize.y);
    cout << "Playfield size: "   << mConfig->fieldSize   << endl;
    cout << "Playfield origin: " << mConfig->fieldOrigin << endl;
    cout << "Playfield rect: "   << mConfig->fieldRect   << endl;
}
