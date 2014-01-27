/*
 *  Tile.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#include "Tile.h"
#include "LVSEngine.h"
#include "Player.h"
#include "Fader.h"
#include "FaderPack.h"
#include "World.h"
#include "OverlayFxManager.h"
#include "TileLevel.h"
#include "AudioEngine.h"

#define FADEFADER 0
#define GROWFADER 1
#define MOVEFADER 2

// Init defaults
void Tile::setup( Configuration *config, const Vec2i newPos, int newType, bool grow )
{
    mConfig = config;
    
    pos = new Vec2i( newPos.x, newPos.y );
    active = false;
    selected = false;
    surrounding = false;
    type = newType;

    moving = false;
    dead = false;
    fading = false;
    shrinking = false;
    finished = false;

    fadeFader = config->faders->createFader(FADEFADER, &baseAlpha, this);
    growFader = config->faders->createFader(GROWFADER, &growPos, this);
    moveFader = config->faders->createFader(MOVEFADER, &movePos, this);

    if (grow) {
        baseTileSize = 0;
        baseAlpha = 0.0;
        tileSize = 0;
        growing = true;
        growPos = 0.0;
        growFader->fade(1.0, GROW_TIME_SEC);
    }
    else {
        baseTileSize = mConfig->tileSize;
        tileSize = mConfig->tileSize;
        growing = false;
        baseAlpha = 1.0;
    }

    for (int i = 0; i < FILTER_SIZE; i++) { prevTileSize[i] = tileSize; }

    drawPos = getScreenPositionVector();
    
    paused = false;
}

bool Tile::selectable() { return (!dead && !moving && !growing); }
void Tile::regrow() { setup(mConfig, *pos, mConfig->world->rndTileType(), true); }
void Tile::toggleSelected() { if (!dead && !moving && !growing ) { selected = !selected; } }
void Tile::setSurrounding(bool value) { surrounding = value; }

void Tile::shutdown()
{
    mConfig->faders->deleteFader(fadeFader);
    mConfig->faders->deleteFader(growFader);
    mConfig->faders->deleteFader(moveFader);
}

void Tile::pause()
{
    fadeFader->pause();
    growFader->pause();
    moveFader->pause();
    paused = true;
}

void Tile::resume()
{
    fadeFader->resume();
    growFader->resume();
    moveFader->resume();
    paused = false;
}

int Tile::kill(int mult)
{
    if (paused) return 0;
    
    if (moving || dead || growing || shrinking) { return 0; }
    
    dead = true;
    mConfig->levels[type]->addScore(mult);
    mConfig->overlayFx->createExplosion(getScreenPositionVector(Vec2i(pos->x, pos->y), false), ColorA(1.0, 1.0, 1.0, 1.0));
    mConfig->audio->e_tileDestroy(type);
    type = -1;
    
    fadeFader->fade(0.0, FADE_TIME_SEC);
    fading = true;
    return mult;
}

// Game over, shrink the tile
void Tile::shrink()
{
    if (paused) return;

    fadeFader->fade(0.0, SHRINK_TIME_SEC);
    fading = true;
    
    moveSrcPos = Vec2i(pos->x, pos->y);
    moveDestPos = Vec2i(pos->x, pos->y);

    moveSrcLoc = getScreenPositionVector(moveSrcPos);
    moveDestLoc = getWindowCenter();
    
    growFader->fade(0.0, SHRINK_TIME_SEC);
    growing = true;
    
    movePos = 0.0;
    moveFader->fade(1.0, SHRINK_TIME_SEC);
    moving = true;
    
    shrinking = true;
}

// Starts moving the tile to another position
void Tile::moveTo( Vec2i newPos )
{
    if (paused) return;

    if (!dead && !moving && !growing) {
        moving = true;

        moveSrcPos = Vec2i(pos->x, pos->y);
        moveDestPos = Vec2i(newPos.x, newPos.y);
        moveSrcLoc = getScreenPositionVector(moveSrcPos);
        moveDestLoc = getScreenPositionVector(moveDestPos);

        baseAlpha = 0.5;
        
        movePos = 0.0;
        moveFader->fade(1.0, MOVE_TIME_SEC);
        
        mConfig->audio->e_tileMove(Vec2i(pos->x, pos->y));
    }
}

void Tile::update( bool hovering, const float dist, const float modifier )
{
    if (paused) return;

    if (!dead && !finished && mConfig->levels[type]->isFinished()) {
        finished = true;
        finishAlpha = baseAlpha;
        // fadeFader->fade(0.0, SHRINK_TIME_SEC);
    }
    
    drawPos = getScreenPositionVector();

    if (dead && !fading) { regrow(); }

    if (growing) {
        baseAlpha = growPos;
        baseTileSize = growPos * mConfig->tileSize;
        tileSize = growPos * mConfig->tileSize;
    }
    if (moving) {
        drawPos = moveSrcLoc - ((moveSrcLoc - moveDestLoc) * movePos);
    }

    if (!dead) {
        // Activate if cursor is over the hex and hex is not moving
        active = (dist < (0.9 * baseTileSize) && !shrinking && !moving && !growing && (hovering || surrounding)) ? true : false;
        
        // Shift averages
        for (int i = 1; i < FILTER_SIZE; i++) { prevTileSize[i - 1] = prevTileSize[i]; }
        
        // Init the new tile size
        prevTileSize[FILTER_SIZE - 1] = math<float>::clamp(mConfig->tileUpperLimit * (modifier * 16), (baseTileSize / 4), mConfig->tileUpperLimit);
        
        // Calculate moving average
        float sum = 0;
        for (int i = 0; i < FILTER_SIZE; i++) { sum += prevTileSize[i]; }
        tileSize = (sum / FILTER_SIZE);        
    }
}

inline Vec2f Tile::getScreenPositionVector(Vec2i loc, bool shiftOrigin)
{
    float x = mConfig->padding_x + (mConfig->tileGrid / 2) + loc.x * (1.5 * mConfig->tileGrid);
    float y = mConfig->padding_y + (mConfig->tileGrid / 2) + loc.y * (0.43 * mConfig->tileGrid) + 7;
    if (loc.y % 2 != 0) { x += (0.74 * mConfig->tileGrid); }
    
    if (shiftOrigin) {
        x += mConfig->fieldOrigin.x;
        y += mConfig->fieldOrigin.y;        
    }
    
    return Vec2f(x, y);
}

// Returns the screen position of this hex
Vec2f Tile::getScreenPositionVector() { return getScreenPositionVector(Vec2i(pos->x, pos->y)); }

// Draws the tile
void Tile::draw()
{
    if (paused) return;

    if (baseAlpha > 0) {
        float lightness = sin(getElapsedSeconds() / 10);
        
        if (!dead) { drawAlive(lightness); }
        else { drawDead(lightness); }
        
        if (growing) { drawGrow(lightness); }
    }
    
#ifdef DEBUG
    drawLabel(drawPos);
#endif
}

// Draw alive hex
inline void Tile::drawAlive(float lightness)
{
    float val = sin((getElapsedSeconds() * 2 + sin(drawPos.x) + cos(drawPos.y))) * 0.2f + 0.5f;
    
//    gl::color(0, 0, lightness * 0.4, 0.75 * baseAlpha);
//    gl::drawSolidCircle( drawPos, mConfig->tileSize, 6 );
    mConfig->engine->bufferSolidCircle( drawPos, mConfig->tileSize, 6, ColorA(0, 0, lightness * 0.4, 0.75 * baseAlpha) );
    mConfig->engine->bufferStrokedCircle( drawPos, baseTileSize, 6, ColorA(0.8, 0.5, 1.0, 0.25 * baseAlpha) );
    
    gl::color (1.0, 1.0, 1.0, val * baseAlpha);
    
#ifndef DEBUG
    if (type == 0) drawHex(drawPos, val, mConfig->levels[0]->getLevel());
    else if (type == 1) drawStar(drawPos, val, mConfig->levels[1]->getLevel());
    else if (type == 2) drawGram(drawPos, val, mConfig->levels[2]->getLevel());
    else if (type == 3) drawPlant(drawPos, val, mConfig->levels[3]->getLevel());
    
    if (active) drawActive(drawPos, val);
    if (selected) drawSelected(drawPos, val);
    if (surrounding && !growing) drawSurrounding(drawPos, val);
#endif
    
    // Drop shadow
    drawDropShadow();
    if (finished) drawFinished();
}

inline void Tile::drawDropShadow()
{
    /*int segments = 6;
    ColorA color = ColorA(0.5, 0.25, 0.75, 0.10);
    for (int i = 1; i < 10; i++) {
        color.a = color.a * 0.80;
        mConfig->engine->bufferStrokedCircle( drawPos, mConfig->tileSize + i, segments, color );
    }*/
}

inline void Tile::drawDead(float lightness)
{
    mConfig->engine->bufferStrokedCircle( drawPos, baseTileSize * 0.8, 6, ColorA(0.4, 0.25, lightness * 0.8, 0.6 * baseAlpha) );
    mConfig->engine->bufferStrokedCircle( drawPos, baseTileSize * 0.6, 6, ColorA(0.3, 0.2, lightness * 0.7, 0.5 * baseAlpha) );
    mConfig->engine->bufferStrokedCircle( drawPos, baseTileSize * 0.4, 6, ColorA(0.2, 0.1, lightness * 0.6, 0.4 * baseAlpha) );
}

inline void Tile::drawFinished()
{
    if (finishAlpha > 0.03) {
//        gl::color (1.0, 1.0, 1.0, finishAlpha);
//        gl::drawSolidCircle( drawPos, baseTileSize, 6 );
        mConfig->engine->bufferSolidCircle( drawPos, baseTileSize, 6, ColorA(1.0, 1.0, 1.0, finishAlpha) );
        mConfig->engine->bufferStrokedCircle( drawPos, baseTileSize + ((1 - finishAlpha) * 20), 6, ColorA(1.0, 1.0, 1.0, finishAlpha) );
        finishAlpha *= 0.9;
    }
}

inline void Tile::drawGrow(float lightness)
{
    // Not used after all
}

// Draw debug label
inline void Tile::drawLabel(Vec2f draw_pos)
{
    gl::drawStringCentered ("x:" + toString(pos->x) + " y:" + toString(pos->y), draw_pos);
}

// Draw the green plant tile (render world when ready)
inline void Tile::drawPlant(Vec2f draw_pos, float val, int level) {
    if (level == 0) {
        int segments = 8;
        for (int i = 0; i < FILTER_SIZE; i++) {
            float a = i * 1.0 / FILTER_SIZE;
            ColorA color;
            if (growing) {
                color = ColorA(0.5 * val * i, 0.5 * val * i, 0.5 * val * i, a * baseAlpha * 0.25);
            }
            else {
                color = ColorA(0.5, 0.25 * val * i, 0.5, a * baseAlpha * 0.25);
            }
            
            mConfig->engine->bufferStrokedCircle( draw_pos, prevTileSize[i] - (FILTER_SIZE - i) * (mConfig->tileBorderSpacing / 2) * 0.75, segments, color );
        }
    }
    
    else if (level == 1) {
        int segments = 4;
        for (int i = 0; i < FILTER_SIZE; i++) {
            float a = i * 1.0 / FILTER_SIZE;
            ColorA color;
            if (growing) {
                color = ColorA(0.5 * val * i, 0.5 * val * i, 0.5 * val * i, a * baseAlpha * 0.2);
            }
            else {
                color = ColorA(0.25, 0.75 * val * i, 0.4, a * baseAlpha * 0.5);
            }
            mConfig->engine->bufferStrokedCircle( draw_pos, prevTileSize[i] - (FILTER_SIZE - i) * (mConfig->tileBorderSpacing / 2) * 0.75, segments, color );
        }       
    }

    else if (level == 3) {
        int segments = 16;
        float slimness = 0.66;
        int n = FILTER_SIZE - 4;
        for (int i = 3; i < n - 2; i++) {
            float a = i * 1.0 / n + 0.1;
            ColorA color;
            if (growing) {
                color = ColorA(0.5 * val * i, 0.5 * val * i, 0.5 * val * i, a * baseAlpha * 0.2);
            }
            else {
                color = ColorA(0.6,
                               0.75 * val * i,
                               0.2,
                               a * baseAlpha * 0.6);
            }
            mConfig->engine->bufferStrokedStar( draw_pos, prevTileSize[i] - (n - i) * (mConfig->tileBorderSpacing) * 0.6, segments, slimness, color );
        }
    }

    else if (level == 2) {
        int segments = 12;
        float slimness = 0.60;
        int n = FILTER_SIZE - 6;
        for (int i = 2; i < n - 2; i++) {
            float a = i * 1.0 / n + 0.1;
            ColorA color;
            if (growing) {
                color = ColorA(0.5 * val * i, 0.5 * val * i, 0.5 * val * i, a * baseAlpha * 0.2);
            }
            else {
                color = ColorA(0.5,
                               0.75 * val * i,
                               0.2,
                               a * baseAlpha * 0.55);
            }
            mConfig->engine->bufferStrokedStar( draw_pos, prevTileSize[i] - (n - i) * (mConfig->tileBorderSpacing) * 0.6, segments, slimness, color );
        }
    }

    else if (finished) {
        int segments = 6;
        ColorA color = ColorA(0.25, 0.75, 0.25, 0.33);
        mConfig->engine->bufferStrokedCircle( draw_pos, baseTileSize, segments, color );
    }
}

// Draw the red tile (render world when ready)
inline void Tile::drawGram(Vec2f draw_pos, float val, int level)
{
    if (level == 0) {
        int segments = 6;
        for (int i = 0; i < FILTER_SIZE; i++) {
            float a = i * 1.0 / FILTER_SIZE;
            ColorA color;
            if (growing) {
                color = ColorA(0.1 * (i / FILTER_SIZE) * val, 0.1 * (i / FILTER_SIZE) * val, 0.1 * (i / FILTER_SIZE) * val, a * baseAlpha);
            }
            else {
                color = ColorA(0.4, 0.1 * (i / FILTER_SIZE) * val, 1.0 - val * 0.25 * i, a * baseAlpha);
            }
            mConfig->engine->bufferStrokedCircle( draw_pos, prevTileSize[i] - (FILTER_SIZE - i) * (mConfig->tileBorderSpacing / 2), segments, color );
        }
    }
    
    else if (level == 1) {
        int segments = 4;
        for (int i = 0; i < FILTER_SIZE; i++) {
            float a = i * 1.0 / FILTER_SIZE;
            ColorA color;
            if (growing) {
                color = ColorA(0.1 * (i / FILTER_SIZE) * val, 0.1 * (i / FILTER_SIZE) * val, 0.1 * (i / FILTER_SIZE) * val, a * baseAlpha * 0.75);
            }
            else {
                color = ColorA(0.6, 0.1 * (i / FILTER_SIZE) * val, 1.0 - val * 0.25 * i, a * baseAlpha * 0.75);
            }
            mConfig->engine->bufferStrokedCircle( draw_pos, prevTileSize[i] - (FILTER_SIZE - i) * (mConfig->tileBorderSpacing / 2) * 0.75, segments, color );
        }        
    }

    else if (level == 3) {
        int n = FILTER_SIZE - 2;
        float slimness = 0.7;
        int numSegments = 16;
        for (int i = 0; i < n; i++) {
            float a = (i * 1.0 / n) + 0.1;
            ColorA color;
            if (growing) {
                color = ColorA(0.1 * (i / n) * val, 0.1 * (i / n) * val, 0.1 * (i / n) * val, a * baseAlpha * 0.75);
            }
            else {
                color = ColorA(0.7,
                               0.4 * (i / n) * val,
                               0.1,
                               a * baseAlpha * 0.666);
            }
            mConfig->engine->bufferStrokedStar( draw_pos, prevTileSize[i] - (n - i) * (mConfig->tileBorderSpacing) * 0.42, numSegments, slimness, color );
        }
    }

    else if (level == 2) {
        int n = FILTER_SIZE - 4;
        float slimness = 0.63;
        int numSegments = 12;
        for (int i = 0; i < n; i++) {
            float a = (i * 1.0 / n) - 0.1;
            ColorA color;
            if (growing) {
                color = ColorA(0.1 * (i / n) * val, 0.1 * (i / n) * val, 0.1 * (i / n) * val, a * baseAlpha * 0.75);
            }
            else {
                color = ColorA(0.8,
                               0.5 * (i / n) * val,
                               0.1 * val,
                               a * baseAlpha * 0.7);
            }
            mConfig->engine->bufferStrokedStar( draw_pos, prevTileSize[i] - (n - i) * (mConfig->tileBorderSpacing) * 0.65, numSegments, slimness, color );
        }
    }
    
    else if (finished) {
        int segments = 6;
        ColorA color = ColorA(0.5, 0.1, 0.1, 0.33);
        mConfig->engine->bufferStrokedCircle( draw_pos, baseTileSize, segments, color );
    }
}

// Draw the blue hex tile (render world when ready)
inline void Tile::drawHex(Vec2f draw_pos, float val, int level)
{
    int hexCount = 4;

    if (level == 2) {
        int segments = 12;
        float slimness = 0.50;
        for (int i = 0; i < FILTER_SIZE; i++) {
            float a = i * 1.0 / FILTER_SIZE;
            for (int j = 0; j < hexCount; j++) {
                ColorA color;
                if (growing) {
                    color = ColorA(0.1 * j * val, 0.1 * j * val, 0.1 * j * val, 0.25 * a * baseAlpha);
                }
                else {
                    color = ColorA((i / FILTER_SIZE) * 0.25,
                                   0.1 * j * val,
                                   1.0 - val * 0.25 * j,
                                   0.5 * a * baseAlpha);
                }
                
                mConfig->engine->bufferStrokedStar( draw_pos, prevTileSize[i] - (hexCount - j) * mConfig->tileBorderSpacing * 1.1, segments, slimness, color );
            }
        }
    }
    
    else if (level == 3) {
        int segments = 16;
        float slimness = 0.62;
        hexCount = 5;
        for (int i = 0; i < FILTER_SIZE; i++) {
            float a = i * 1.0 / FILTER_SIZE;
            for (int j = 0; j < hexCount; j++) {
                ColorA color;
                if (growing) {
                    color = ColorA(0.1 * j * val, 0.1 * j * val, 0.1 * j * val, 0.25 * a * baseAlpha);
                }
                else {
                    color = ColorA((1 - (i * 1.0 / FILTER_SIZE)) * 0.15,
                                   0,
                                   1.0 - val * 0.25 * j,
                                   0.5 * a * baseAlpha);
                }
                
                mConfig->engine->bufferStrokedStar( draw_pos, prevTileSize[i] - (hexCount - j) * mConfig->tileBorderSpacing, segments, slimness, color );
            }
        }
    }
    
    else if (level == 1) {
        int segments = 4;
        for (int i = 0; i < FILTER_SIZE; i++) {
            float a = i * 1.0 / FILTER_SIZE;
            for (int j = 0; j < hexCount; j++) {
                ColorA color;
                if (growing) {
                    color = ColorA(0.1 * j * val, 0.1 * j * val, 0.1 * j * val, 0.25 * a * baseAlpha);
                }
                else {
                    color = ColorA((i / FILTER_SIZE) * 0.25, 0.1 * j * val, 1.0 - val * 0.25 * j, 0.5 * a * baseAlpha);
                }
                
                mConfig->engine->bufferStrokedCircle( draw_pos, prevTileSize[i] - (hexCount - j) * mConfig->tileBorderSpacing, segments, color );
            }
        }        
    }
    else if (level == 0) {
        int segments = 6;
        for (int i = 0; i < FILTER_SIZE; i++) {
            float a = i * 1.0 / FILTER_SIZE;
            for (int j = 0; j < hexCount; j++) {
                ColorA color;
                if (growing) {
                    color = ColorA(0.1 * j * val, 0.1 * j * val, 0.1 * j * val, 0.25 * a * baseAlpha);
                }
                else {
                    color = ColorA((i / FILTER_SIZE) * 0.25, 0.1 * j * val, 1.0 - val * 0.25 * j, 0.5 * a * baseAlpha);
                }
                
                mConfig->engine->bufferStrokedCircle( draw_pos, prevTileSize[i] - (hexCount - j) * mConfig->tileBorderSpacing, segments, color );
            }
        }        
    }
    
    else if (finished) {
        int segments = 6;
        ColorA color = ColorA(0.1, 0.1, 0.8, 0.33);
        mConfig->engine->bufferStrokedCircle( draw_pos, baseTileSize, segments, color );
    }
}

// Draw a star tile
inline void Tile::drawStar(Vec2f draw_pos, float val, int level)
{
    if (finished) {
        int segments = 6;
        ColorA color = ColorA(0.8, 0.8, 0.8, 0.33);
        mConfig->engine->bufferStrokedCircle( draw_pos, baseTileSize, segments, color );
        
        return;
    }

    for (int i = 0; i < LINE_COUNT; i++) {
        ColorA colorStart;
        ColorA colorEnd;
        
        // Gradient color lines
        float a = (i % 2 == 0) ? 0.5 : 0.25;
        if (growing) {
            colorStart = ColorA(0.2, 0.2, 0.2, a * baseAlpha * 0.33);
            colorEnd   = ColorA(1.0, 1.0, 1.0, a * baseAlpha * 0.33);
        }
        else {
            if (level == 3) {
                colorStart = ColorA(1.0, 0.5, 0.0, a * baseAlpha);
                colorEnd   = ColorA(1.0, 1.0, 1.0, a * baseAlpha);
            }
            else if (level == 2) {
                colorStart = ColorA(0.4, 0.4, 0.2, a * baseAlpha);
                colorEnd   = ColorA(1.0, 1.0, 1.0, a * baseAlpha);
            } // 1
            else if (level == 1) {
                colorStart = ColorA(0.2, 0.2, 0.2, a * baseAlpha);
                colorEnd   = ColorA(1.0, 1.0, 1.0, a * baseAlpha);
            } // 0
            else if (level == 0) {
                colorStart = ColorA(0.0, 0.6, 1.0, a * baseAlpha);
                colorEnd   = ColorA(0.8, 0.6, 0.8, a * baseAlpha);
            }
        }
        
        // TODO: precalc!
        float angle = (i / (1.0 * LINE_COUNT)) * (2 * M_PI);
        float x = draw_pos.x + tileSize * 1.5 * cos(angle);
        float y = draw_pos.y + tileSize * 1.5 * sin(angle);
        
        mConfig->engine->bufferLine(Vec2f(x,y), Vec2f(draw_pos.x, draw_pos.y), colorStart, colorEnd);
    }
    
}

// Draw active tile highlight
inline void Tile::drawActive(Vec2f draw_pos, float val)
{
    mConfig->engine->bufferStrokedCircle( draw_pos, baseTileSize, 6, ColorA(1, 1, 1, 0.5 * baseAlpha) );
    mConfig->engine->bufferSolidCircle(draw_pos, baseTileSize, 6, ColorA(0.6, val, 0.8, 0.4 * baseAlpha));

}

// Draw selected tile highlight
inline void Tile::drawSelected(Vec2f draw_pos, float val)
{
    mConfig->engine->bufferSolidCircle(draw_pos, baseTileSize, 6, ColorA(1.0, 0.2, 0.8, 0.40 * baseAlpha));
    mConfig->engine->bufferStrokedCircle( draw_pos, baseTileSize, 6, ColorA(0.75, 0.66, 0.75, 0.75) );
}

// Draw surrounding tile highlight
inline void Tile::drawSurrounding(Vec2f draw_pos, float val)
{
    mConfig->engine->bufferStrokedCircle( draw_pos, baseTileSize, 6, ColorA(0.75, 0.66, 0.75, 0.75) );
}

void Tile::onFadeEnd(int typeId)
{
    switch (typeId) {
        case FADEFADER:
            fading = false;
            break;
        case GROWFADER:
            growing = false;
            baseAlpha = 1;
            baseTileSize = mConfig->tileSize;
            tileSize = mConfig->tileSize;
            break;
        case MOVEFADER:
            moving = false;
            pos->x = moveDestPos.x;
            pos->y = moveDestPos.y;
            drawPos = getScreenPositionVector();
            baseAlpha = 1.0;
            break;
    }
}



