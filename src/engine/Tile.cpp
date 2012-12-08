/*
 *  Tile.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#include "Tile.h"

// Init defaults
void Tile::setup( Configuration *config, const Vec2i newPos, int newType, bool grow )
{
    mConfig = config;
    
    pos = new Vec2i( newPos.x, newPos.y );
        
    active = false;
    selected = false;
    type = newType;

    moveTimer = new Timer();
    moving = false;
    dead = false;

    fadeTimer = new Timer();
    fading = false;
    
    growTimer = new Timer();
    if (grow) {
        baseTileSize = 0;
        tileSize = 0;
        growing = true;
        growTimer->start();
        baseAlpha = 0.0;
    }
    else {
        baseTileSize = mConfig->tileSize;
        tileSize = mConfig->tileSize;
        growing = false;
        baseAlpha = 1.0;
    }

    for (int i = 0; i < FILTER_SIZE; i++) { prevTileSize[i] = tileSize; }

    drawPos = getScreenPositionVector();
}

// Activates / deactivates selected hex
void Tile::toggleSelected() {
    if (!dead && !moving && !growing ) { selected = !selected; }
}

bool Tile::selectable() { return (!dead && !moving && !growing); }

void Tile::regrow() { setup(mConfig, *pos, mConfig->world->rndTileType(), true); }

int Tile::kill() {
    if (moving || dead || growing) {
        return 0;
    }
    dead = true;
    mConfig->player->addScore(1, type);
    type = -1;
    fadeTimer->start();
    fading = true;
    
    return 1;
}

// Starts moving the tile to another position
void Tile::moveTo( Vec2i newPos ) {
    if (!dead && !moving && !growing) {
        moving = true;
        moveTimeSec = MOVE_TIME_SEC;

        moveSrcPos = Vec2i(pos->x, pos->y);
        moveDestPos = Vec2i(newPos.x, newPos.y);
        
        moveSrcLoc = getScreenPositionVector(moveSrcPos);
        moveDestLoc = getScreenPositionVector(moveDestPos);

        baseAlpha = 0.5;
        moveTimer->start();
    }
}

void Tile::update( const float dist, const float modifier )
{
    if (growing && !growTimer->isStopped()) {
        double time = growTimer->getSeconds();
        if (time >= GROW_TIME_SEC) {
            growing = false;
            baseAlpha = 1;
            growTimer->stop();
            baseTileSize = mConfig->tileSize;
            tileSize = mConfig->tileSize;
        }
        else {
            baseAlpha = time / GROW_TIME_SEC;
            baseTileSize = time / GROW_TIME_SEC * mConfig->tileSize;
            tileSize = time / GROW_TIME_SEC * mConfig->tileSize;
        }
    }
    if (fading && !fadeTimer->isStopped()) {
        double time = fadeTimer->getSeconds();
        if (time >= FADE_TIME_SEC) {
            fading = false;
            baseAlpha = 0;
            fadeTimer->stop();
            regrow();
        }
        else {
            baseAlpha = 1 - (time / FADE_TIME_SEC);
        }
    }
    if (moving && !moveTimer->isStopped()) {
        double time = moveTimer->getSeconds();
        if (time >= moveTimeSec) {
            // Correct final draw pos
            moving = false;
            moveTimer->stop();
            
            pos->x = moveDestPos.x;
            pos->y = moveDestPos.y;
            drawPos = getScreenPositionVector();
            
            baseAlpha = 1.0;
        }
        else {
            double movePoint = time / moveTimeSec;
            drawPos = moveSrcLoc - ((moveSrcLoc - moveDestLoc) * movePoint);
            // Check move vector directions
            // Update screen position
        }
    }

    if (!dead) {
        // Activate if cursor is over the hex and hex is not moving
        active = (dist < (0.9 * baseTileSize) && !moving && !growing) ? true : false;
        
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

Vec2f Tile::getScreenPositionVector(Vec2i loc) {
    float x = mConfig->padding + (mConfig->tileGrid / 2) + loc.x * (1.5 * mConfig->tileGrid);
    float y = mConfig->padding + (mConfig->tileGrid / 2) + loc.y * (0.43 * mConfig->tileGrid);
    if (loc.y % 2 != 0) { x += (0.74 * mConfig->tileGrid); }
    
    return Vec2f(x, y);
}

// Returns the screen position of this hex
inline Vec2f Tile::getScreenPositionVector() { return getScreenPositionVector(Vec2i(pos->x, pos->y)); }

// Draw alive hex
inline void Tile::drawAlive(float lightness) {
    float val = sin((getElapsedSeconds() * 2 + sin(drawPos.x) + cos(drawPos.y))) * 0.2f + 0.5f;
    
    gl::color(0, 0, lightness * 0.4, 0.75 * baseAlpha);
    gl::drawSolidCircle( drawPos, mConfig->tileSize, 6 );
    
    mConfig->world->addCirclePoly( drawPos, baseTileSize, 6, ColorA(0.8, 0.5, 1.0, 0.25 * baseAlpha) );
    
    gl::color (1.0, 1.0, 1.0, val * baseAlpha);
    
#ifndef DEBUG
    if (type == 0) drawHex(drawPos, val);
    else if (type == 1) drawStar(drawPos, val);
    else if (type == 2) drawGram(drawPos, val);
    else if (type == 3) drawPlant(drawPos, val);
    
    if (active) drawActive(drawPos, val);
    if (selected) drawSelected(drawPos, val);
#endif
}

inline void Tile::drawDead(float lightness) {
    mConfig->world->addCirclePoly( drawPos, baseTileSize * 0.8, 6, ColorA(0.8, 0.1, lightness * 0.8, 0.6 * baseAlpha) );
    mConfig->world->addCirclePoly( drawPos, baseTileSize * 0.6, 6, ColorA(0.7, 0.1, lightness * 0.7, 0.5 * baseAlpha) );
    mConfig->world->addCirclePoly( drawPos, baseTileSize * 0.4, 6, ColorA(0.6, 0.1, lightness * 0.6, 0.4 * baseAlpha) );
}

inline void Tile::drawGrow(float lightness) {
    // XXX
}

// Draws the tile
void Tile::draw()
{
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

// Draw debug label
inline void Tile::drawLabel(Vec2f draw_pos) {
    gl::drawStringCentered ("x:" + toString(pos->x) + " y:" + toString(pos->y), draw_pos);
}

// Draw the green plant tile (render world when ready)
inline void Tile::drawPlant(Vec2f draw_pos, float val) {
    int segments = 8;
    
    for (int i = 0; i < FILTER_SIZE; i++) {
        float a = i * 1.0 / FILTER_SIZE;
        ColorA color = ColorA(0.5, 0.25 * val * i, 0.5, a * baseAlpha * 0.25);
        mConfig->world->addCirclePoly( draw_pos, prevTileSize[i] - (FILTER_SIZE - i) * (mConfig->tileBorderSpacing / 2) * 0.75, segments, color );
    }
}

// Draw the red tile (render world when ready)
inline void Tile::drawGram(Vec2f draw_pos, float val) {
    int segments = 6;
    
    for (int i = 0; i < FILTER_SIZE; i++) {
        float a = i * 1.0 / FILTER_SIZE;
        ColorA color = ColorA(0.4, 0.1 * (i / FILTER_SIZE) * val, 1.0 - val * 0.25 * i, a * baseAlpha);
        mConfig->world->addCirclePoly( draw_pos, prevTileSize[i] - (FILTER_SIZE - i) * (mConfig->tileBorderSpacing / 2), segments, color );
    }
}

// Draw the blue hex tile (render world when ready)
inline void Tile::drawHex(Vec2f draw_pos, float val)
{
    int hexCount = 4;
    int segments = 6;
    
    for (int i = 0; i < FILTER_SIZE; i++) {
        float a = i * 1.0 / FILTER_SIZE;
        for (int j = 0; j < hexCount; j++) {
            ColorA color = ColorA((i / FILTER_SIZE) * 0.25, 0.1 * j * val, 1.0 - val * 0.25 * j, 0.5 * a * baseAlpha);
            mConfig->world->addCirclePoly( draw_pos, prevTileSize[i] - (hexCount - j) * mConfig->tileBorderSpacing, segments, color );
        }
    }
}

// Draw a star tile
inline void Tile::drawStar(Vec2f draw_pos, float val)
{
    GLfloat lines[LINE_COUNT * 4];
    GLfloat colors[LINE_COUNT * 8];

    for (int i = 0; i < LINE_COUNT; i++) {
        int lineIdx = i * 4;

        // Gradient color lines
        float a = (i % 2 == 0) ? 0.5 : 0.25;
        colors[2 * lineIdx + 0] = 0.0; colors[2 * lineIdx + 1] = 0.6;
        colors[2 * lineIdx + 2] = 1.0; colors[2 *lineIdx + 3] = a * baseAlpha;
        colors[2 * lineIdx + 4] = 1.0; colors[2 * lineIdx + 5] = 0.6;
        colors[2 * lineIdx + 6] = 1.0; colors[2 * lineIdx + 7] = a * baseAlpha;
        
        float angle = (i / (1.0 * LINE_COUNT)) * (2 * M_PI);
        float x = draw_pos.x + tileSize * 1.5 * cos(angle);
        float y = draw_pos.y + tileSize * 1.5 * sin(angle);

        lines[lineIdx] = x; lines[lineIdx + 1] = y;
        lines[lineIdx + 2] = draw_pos.x; lines[lineIdx + 3] = draw_pos.y;        
    }
    
    drawLines(lines, colors, LINE_COUNT);
}

// Draw active tile highlight
inline void Tile::drawActive(Vec2f draw_pos, float val) {
    mConfig->world->addCirclePoly( draw_pos, baseTileSize, 6, ColorA(1, 1, 1, 0.5 * baseAlpha) );
    
    gl::color (0.6, val, 0.8, 0.4 * baseAlpha);
    gl::drawSolidCircle( draw_pos, baseTileSize, 6 );
}

// Draw selected tile highlight
inline void Tile::drawSelected(Vec2f draw_pos, float val) {
    gl::color (1.0, 0.2, 0.8, 0.40 * baseAlpha);
    gl::drawSolidCircle( draw_pos, baseTileSize, 6 );
}
