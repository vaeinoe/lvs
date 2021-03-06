/*
 *  LVSEngine.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#define S_LOADING      0
#define S_MAINMENU     1
#define S_INSTRUCTIONS 2
#define S_INGAME_1     3
#define S_GAMEOVER     4
#define S_VICTORY      5
#define S_QUITTING     6
#define S_OVER         7

#define FADEFADER 0
#define GAMEFADER 1

#include "LVSEngine.h"
#include "World.h"
#include "Toolbar.h"
#include "AudioEngine.h"
#include "Mainmenu.h"
#include "FaderPack.h"
#include "Fader.h"
#include "OverlayFxManager.h"
#include "TileLevel.h"
#include "HighScores.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

#include "../LeavsApp.h"

void LVSEngine::setup(Lvs *lvsApp, Configuration *config)
{
	mApp = lvsApp;

    fadeVal = 1.0;
    timeRunningOut = false;

    mConfig = config;
    mAudio = new AudioEngine();
    mToolbar = new Toolbar();
    mWorld = new World();
    mMenu = new Mainmenu();
    mFaders = new FaderPack();
    mHiScores = new HighScores();
    
    overlayFx = new OverlayFxManager();

    mConfig->engine = this;
    mConfig->toolbar = mToolbar;
    mConfig->world = mWorld;
    mConfig->faders = mFaders;
    mConfig->audio = mAudio;
    mConfig->overlayFx = overlayFx;
    mConfig->hiScores = mHiScores;

    setPlayfield();
    
    for (int i = 0; i < config->numTileTypes; i++) {
        mConfig->levels[i] = new TileLevel();
        mConfig->levels[i]->setup(mConfig, i);
    }

    dragging = false;

    mMouseLoc = new Vec2i(0, 0);

    gameState = S_LOADING;
    paused = false;

    Font loadFont = Font( GAME_FONT, FONT_SIZE_SMALL);
    mConfig->fontSmall = gl::TextureFont::create(loadFont);
    loadFont = Font( GAME_FONT, FONT_SIZE_MEDIUM);
    mConfig->fontMedium = gl::TextureFont::create(loadFont);
    loadFont = Font( GAME_FONT, FONT_SIZE_LARGE);
    mConfig->fontLarge = gl::TextureFont::create(loadFont);

    loadState = 0;
    loadStr = "mixing cupcake batter";
    
    instructions = ci::loadImage( loadResource( RES_INSTRUCTIONS ) );
    
    fullScreen = false;

    mFaders->setup(mConfig, true);
    screenFader = mFaders->createFader(FADEFADER, &fadeVal, this);

    gameFader = mFaders->createFader(GAMEFADER);
    gameFader->addObserver(this);
}

void LVSEngine::loadAll() {
    switch (loadState) {
        case 0:
            loadStr = "waking up";
            break;
        case 1:
            mToolbar->setup(mConfig, Vec2i(0, 0), Vec2i(getWindowWidth(), mConfig->toolbarHeight));
            mWorld->setup(mConfig, Vec2i(mConfig->worldWidth, mConfig->worldHeight));
            overlayFx->setup(mConfig);
            mHiScores->setup(mConfig);
            mMenu->setup(mConfig);
            mMenu->updateScore();
            mAudio->setup(mConfig);
            precalc();
            loadStr = "slicing the world into shards...";
            break;
        case 2:
            mAudio->initAudio();
            loadStr = "filling the glass half full...";
            break;
        case 3:
            mAudio->initAudio();
            loadStr = "preparing the picardy third...";
            break;
        case 4:
            mAudio->initAudio();
            loadStr = "go.";
            break;
        case 5:
            // cout << "Running." << endl;
            gameState = S_MAINMENU;
            
            screenFader->fade(0.0, 2.5);
            mAudio->e_gameLoaded();
            break;
    }

    loadState++;
}

void LVSEngine::update()
{
	if (gameState == S_OVER) return;

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
            checkTime();
            mAudio->update();
            overlayFx->update();
            mToolbar->update();
            mWorld->update(mMouseLoc, mAudio->getFreqData(), mAudio->getDataSize());
            break;
        case S_QUITTING:
            mAudio->update();
            break;
        case S_VICTORY:
            mAudio->update();
            mWorld->update(mMouseLoc, mAudio->getFreqData(), mAudio->getDataSize());
        default:
            mAudio->update();
            break;
    }
}

inline void LVSEngine::checkTime() {
    double gameTime = getGameTime();
    if (!timeRunningOut && gameTime <= WARN_GAME_TIME && gameTime > 0.0) {
        timeRunningOut = true;
        mAudio->e_timeEnding();
    }
    else if (timeRunningOut && gameTime > WARN_GAME_TIME) {
        timeRunningOut = false;
        mAudio->e_timeNormal();
    }
}

inline void LVSEngine::drawGame(float lightness)
{
    gl::color( 0, 0, lightness * 0.4 );
    gl::drawSolidRect(mConfig->fieldRect);

    if (timeRunningOut) {
        double gameTime = getGameTime();
        double bri = fmod(gameTime, 1);
        double a   = (1 - (gameTime / WARN_GAME_TIME)) * 0.3;
        gl::color(bri, bri / 2, bri, a);
        gl::drawSolidRect(mConfig->fieldRect);
    }
    
    mWorld->draw();
    overlayFx->drawDelayed();
    drawBuffer();
    mToolbar->draw();
    overlayFx->drawImmediate();
}

void LVSEngine::draw()
{
	if (gameState == S_OVER) return;

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
        case S_INSTRUCTIONS:
            gl::color(1,1,1,1.0);
            gl::draw(instructions, mConfig->fieldRect);
            return;
            break;
        case S_QUITTING:
            gl::color( lightness * 0.4, 0, 0.2 );
            gl::drawSolidRect(mConfig->fieldRect);
            mAudio->draw();
            mMenu->draw();
            break;
        case S_VICTORY:
            drawGame(lightness);
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
    if (gameState == S_VICTORY) {
        gl::color(1, 1, 1, fadeVal);
        string score = "time: " + toString(floor(endGameTime + 0.5)) + " seconds.";
        Vec2f winSize     = mConfig->fontMedium->measureString(WINGAME_STR) / 2;
        Vec2f scoreOffset = Vec2f(0, 16);
        Vec2f scoreSize   = mConfig->fontSmall->measureString(score) / 2;
        Vec2f winCenter   = getWindowCenter();
        mConfig->fontMedium->drawString(WINGAME_STR, winCenter - winSize);
        mConfig->fontSmall->drawString(score, winCenter - scoreSize + scoreOffset);
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
        case S_INSTRUCTIONS:
            backToMain(false);
            break;
    }
}

void LVSEngine::keyDown ( const KeyEvent event ) {
    if( gameState == S_INSTRUCTIONS) {
        backToMain(false);
		return;
    }
    
#ifndef TARGET_OS_IPHONE
    if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
        switch (gameState) {
            case S_MAINMENU:
                quitGame();
                break;
            case S_INGAME_1:
                backToMain();
                break;
        }
    }
    else if(event.getCode() == KeyEvent::KEY_s) {
        string filename = toString(std::time(0)) + "_screen.png";
        writeImage( filename, copyWindowSurface() );
    }
#endif
#ifdef __APPLE__
#ifndef TARGET_OS_IPHONE
    else if(event.getCode() == KeyEvent::KEY_f) {
		if (gameState > S_LOADING && gameState <= S_QUITTING) {
	        toggleFullscreen();
		}
    }
#endif
#endif
}

bool LVSEngine::isVictory()
{
    for (int i = 0; i < mConfig->numTileTypes; i++) {
        if (!mConfig->levels[i]->isFinished()) {
            return false;
        }
    }
    
    return true;
}

inline bool LVSEngine::checkVictory()
{
    if (gameState == S_INGAME_1 && isVictory()) {
        gameFader->stop();
        gameState = S_VICTORY;
        mAudio->e_gameWin();
        mWorld->shrink();
        mMenu->setPaused(false);
        screenFader->fade(1.0, SHRINK_TIME_SEC);
                
        endGameTime = gameFader->timeElapsed();
        mHiScores->saveScore(endGameTime);
        mMenu->updateScore();

        return true;
    }
    
    return false;
}

void LVSEngine::startGame()
{
    gameState = S_INGAME_1;
    mMenu->deactivate();
    
    if (paused) {
        // cout << "Resuming" << endl;
        gameFader->resume();
        mWorld->resume();
        mAudio->e_gameResume();
    }
    else {
        mConfig->overlayFx->createText(Vec2f(0,0), ColorA(1.0, 1.0, 1.0, 0.5),
                                       FONT_TYPE_MEDIUM, GAME_START_STR);
        gameFader->fade(0.0, INIT_GAME_TIME);
        mAudio->e_gameStart();
    }
    paused = false;
    mMenu->setPaused(false);
}

void LVSEngine::backToMain(bool fromGame)
{
    gameState = S_MAINMENU;
    mMenu->activate();
    
    if (fromGame) {
        mAudio->e_gamePause();
        mMenu->setPaused(true);
        
        gameFader->pause();
        mWorld->pause();
        
        paused = true;
    }
}

void LVSEngine::showInstructions()
{
    gameState = S_INSTRUCTIONS;
    mMenu->deactivate();
}

void LVSEngine::quitGame()
{
    gameState = S_QUITTING;
    mMenu->deactivate();
    mAudio->e_gameQuit();
    
    screenFader->fade(1.0, 5.0);
}

void LVSEngine::gameOver()
{
    gameState = S_GAMEOVER;
    mAudio->e_gameLose();
    mMenu->setPaused(false);
    screenFader->fade(1.0, 3.0);
}

inline void LVSEngine::resetGame()
{
    mWorld->reset();
    for (int i = 0; i < mConfig->numTileTypes; i++) {
        mConfig->levels[i]->reset();
    }
    mToolbar->reset();
}

void LVSEngine::addGameTime(int seconds) { gameFader->addTime(seconds); }

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
            //shutdown();
            //exit(0);
			gameState = S_OVER;
			mApp->quit();
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
    mAudio->shutdown();
    mToolbar->shutdown();
    mWorld->shutdown();
    mMenu->shutdown();
    mFaders->shutdown();
    mHiScores->shutdown();
    
    delete mToolbar;
    delete mWorld;
    delete mAudio;
    delete mMenu;
    delete mFaders;
    delete mHiScores;
}

void LVSEngine::bufferSolidCircle( const Vec2f &center, const float radius, int numSegments, const ColorA &color )
{
    // automatically determine the number of segments from the circumference
	if( numSegments <= 0 ) {
		numSegments = (int)math<double>::floor( radius * M_PI * 2 );
	}
	if( numSegments < 2 ) numSegments = 2;

    // Triangle segments
	for( int s = 0; s <= numSegments; s++ ) {
        triVerts.push_back(center.x);
        triVerts.push_back(center.y);
        
        triVerts.push_back(center.x + precalcAngles[numSegments][s].x * radius);
        triVerts.push_back(center.y + precalcAngles[numSegments][s].y * radius);

        triVerts.push_back(center.x + precalcAngles[numSegments][(s + 1) % numSegments].x * radius);
        triVerts.push_back(center.y + precalcAngles[numSegments][(s + 1) % numSegments].y * radius);
        
        for (int i = 0; i < 3; i++) {
            triColors.push_back(color.r); triColors.push_back(color.g);
            triColors.push_back(color.b); triColors.push_back(color.a);            
        }
    }
}

// Buffers a single line for drawing
void LVSEngine::bufferLine( const Vec2f start, const Vec2f end,
                            const ColorA colorStart, const ColorA colorEnd )
{
    lineVerts.push_back(start.x); lineVerts.push_back(start.y);
    lineVerts.push_back(end.x); lineVerts.push_back(end.y);
    lineColors.push_back(colorStart.r); lineColors.push_back(colorStart.g);
    lineColors.push_back(colorStart.b); lineColors.push_back(colorStart.a);
    lineColors.push_back(colorEnd.r); lineColors.push_back(colorEnd.g);
    lineColors.push_back(colorEnd.b); lineColors.push_back(colorEnd.a);
}

void LVSEngine::bufferStrokedStar( const Vec2f &center, const float radius, int numSegments,
                                   float slimness, const ColorA &color) {
    GLfloat firstX = center.x + precalcAngles[numSegments][0].x * radius;
    GLfloat firstY = center.y + precalcAngles[numSegments][0].y * radius;
    lineVerts.push_back(firstX); lineVerts.push_back(firstY);
    lineColors.push_back(color.r); lineColors.push_back(color.g);
    lineColors.push_back(color.b); lineColors.push_back(color.a);

	for( int s = 1; s < numSegments; s++ ) {
        float r = (s % 2 == 0) ? radius : radius * slimness;
        for (int i = 0; i < 2; i++) {
            lineVerts.push_back(center.x + precalcAngles[numSegments][s].x * r);
            lineVerts.push_back(center.y + precalcAngles[numSegments][s].y * r);
            lineColors.push_back(color.r); lineColors.push_back(color.g);
            lineColors.push_back(color.b); lineColors.push_back(color.a);
        }
	}
    
    // drawing line loops = back to first vertex in the end
    lineVerts.push_back(firstX); lineVerts.push_back(firstY);
    lineColors.push_back(color.r); lineColors.push_back(color.g);
    lineColors.push_back(color.b); lineColors.push_back(color.a);
}

// Adds a polygon to rendering queue
void LVSEngine::bufferStrokedCircle( const Vec2f &center, const float radius, int numSegments, const ColorA &color )
{
	// automatically determine the number of segments from the circumference
	if( numSegments <= 0 ) {
		numSegments = (int)math<double>::floor( radius * M_PI * 2 );
	}
	if( numSegments < 2 ) numSegments = 2;
    
    // first vertex
    GLfloat firstX = center.x + precalcAngles[numSegments][0].x * radius;
    GLfloat firstY = center.y + precalcAngles[numSegments][0].y * radius;
    lineVerts.push_back(firstX); lineVerts.push_back(firstY);
    lineColors.push_back(color.r); lineColors.push_back(color.g);
    lineColors.push_back(color.b); lineColors.push_back(color.a);
    
    // middle vertices
	for( int s = 1; s < numSegments; s++ ) {
        for (int i = 0; i < 2; i++) {
            lineVerts.push_back(center.x + precalcAngles[numSegments][s].x * radius);
            lineVerts.push_back(center.y + precalcAngles[numSegments][s].y * radius);
            lineColors.push_back(color.r); lineColors.push_back(color.g);
            lineColors.push_back(color.b); lineColors.push_back(color.a);
        }
	}
    
    // drawing line loops = back to first vertex in the end
    lineVerts.push_back(firstX); lineVerts.push_back(firstY);
    lineColors.push_back(color.r); lineColors.push_back(color.g);
    lineColors.push_back(color.b); lineColors.push_back(color.a);
}

// Draws the rendering line queue
inline void LVSEngine::drawBuffer()
{
    glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
    
    // Draw triangles
	glColorPointer( 4, GL_FLOAT, 0, &triColors[0] );
	glVertexPointer( 2, GL_FLOAT, 0, &triVerts[0] );
    glDrawArrays(GL_TRIANGLES, 0, triVerts.size() / 2);

    // Draw lines
	glColorPointer( 4, GL_FLOAT, 0, &lineColors[0] );
	glVertexPointer( 2, GL_FLOAT, 0, &lineVerts[0] );
	glDrawArrays( GL_LINES, 0, lineVerts.size() / 2 );
    
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
    
    lineColors.clear();
    lineVerts.clear();
    
    triColors.clear();
    triVerts.clear();
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
}

// Set playfield parameters - called when switching between fullscreen and windowed
void LVSEngine::toggleFullscreen()
{
    fullScreen = !fullScreen;
    setFullScreen(fullScreen);
    setPlayfield();
}
