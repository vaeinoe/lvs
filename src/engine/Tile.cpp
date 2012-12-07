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
void Tile::setup( Configuration *config, const Vec2i newPos, int newType)
{
    mConfig = config;
    
    pos = new Vec2i( newPos.x, newPos.y );
    tileSize = mConfig->tileSize;
    
    for (int i = 0; i < FILTER_SIZE; i++) { prevTileSize[i] = tileSize; }
    
    active = false;
    selected = false;
    type = newType;

    moveTimer = new Timer();
    moving = false;
    dead = false;

    fadeTimer = new Timer();
    fading = false;
    baseAlpha = 1.0;

    drawPos = getScreenPositionVector();
}

// Activates / deactivates selected hex
void Tile::toggleSelected() {
    if (!dead && !moving) {
        selected = !selected;        
    }
}

bool Tile::selectable() { return (!dead && !moving); }

int Tile::kill() {
    if (moving || dead) {
        return 0;
    }
    dead = true;
    type = -1;
    fadeTimer->start();
    fading = true;
    return 1; // XXX: return score based on size?
}

// Starts moving the tile to another position
void Tile::moveTo( Vec2i newPos ) {
    if (!dead && !moving) {
        moving = true;
        moveTimeSec = MOVE_TIME_SEC;

        moveSrcPos = Vec2i(pos->x, pos->y);
        moveDestPos = Vec2i(newPos.x, newPos.y);
        
        moveSrcLoc = getScreenPositionVector(moveSrcPos);
        moveDestLoc = getScreenPositionVector(moveDestPos);

        moveTimer->start();
    }
}

void Tile::update( const float dist, const float modifier )
{
    if (fading && !fadeTimer->isStopped()) {
        double time = fadeTimer->getSeconds();
        if (time >= FADE_TIME_SEC) {
            fading = false;
            baseAlpha = 0;
            fadeTimer->stop();
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
        active = (dist < (0.9 * mConfig->tileSize) && !moving) ? true : false;
        
        // Shift averages
        for (int i = 1; i < FILTER_SIZE; i++) { prevTileSize[i - 1] = prevTileSize[i]; }
        
        // Init the new tile size
        prevTileSize[FILTER_SIZE - 1] = math<float>::clamp(mConfig->tileUpperLimit * (modifier * 16), (mConfig->tileSize / 4), mConfig->tileUpperLimit);
        
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
Vec2f Tile::getScreenPositionVector() { return getScreenPositionVector(Vec2i(pos->x, pos->y)); }

void Tile::drawAlive(float lightness) {
    float val = sin((getElapsedSeconds() * 2 + sin(drawPos.x) + cos(drawPos.y))) * 0.2f + 0.5f;
    
    gl::color(0, 0, lightness * 0.4, 0.75 * baseAlpha);
    gl::drawSolidCircle( drawPos, mConfig->tileSize, 6 );
    
    gl::color (0.8, 0.5, 1.0, 0.25 * baseAlpha);
    gl::drawStrokedCircle( drawPos, mConfig->tileSize, 6 );
    
    gl::color (1.0, 1.0, 1.0, val * baseAlpha);
    
#ifndef DEBUG
    if (type == 0) drawHex(drawPos, val);
    else if (type == 1) drawStar(drawPos, val);
    else if (type == 2) drawGram(drawPos, val);
    
    if (active) drawActive(drawPos, val);
    if (selected) drawSelected(drawPos, val);
#endif
}

void Tile::drawDead(float lightness) {
    gl::color(0.8, 0.1, lightness * 0.8, 0.6 * baseAlpha);
    gl::drawStrokedCircle( drawPos, mConfig->tileSize * 0.8, 6 );
    gl::color(0.7, 0.1, lightness * 0.7, 0.5 * baseAlpha);
    gl::drawStrokedCircle( drawPos, mConfig->tileSize * 0.6, 6 );
    gl::color(0.6, 0.1, lightness * 0.6, 0.4 * baseAlpha);
    gl::drawStrokedCircle( drawPos, mConfig->tileSize * 0.4,6  );
}

// Draws the tile
void Tile::draw()
{
    if (baseAlpha > 0) {
        float lightness = sin(getElapsedSeconds() / 10);

        if (!dead) { drawAlive(lightness); }
        else { drawDead(lightness); }
    }
    
#ifdef DEBUG
    drawLabel(drawPos);
#endif
}

inline void Tile::drawLabel(Vec2f draw_pos) {
    gl::drawStringCentered ("x:" + toString(pos->x) + " y:" + toString(pos->y), draw_pos);
}


inline void Tile::drawGram(Vec2f draw_pos, float val) {
    int segments = 6;
    
    for (int i = 0; i < FILTER_SIZE; i++) {
        float a = i * 1.0 / FILTER_SIZE;
        gl::color(0.4, 0.1 * (i / FILTER_SIZE) * val, 1.0 - val * 0.25 * i, a * baseAlpha);
        gl::drawStrokedCircle( draw_pos, prevTileSize[i] - (FILTER_SIZE - i) * (mConfig->tileBorderSpacing / 2), segments );
    }
}

// Draw a hex tile
inline void Tile::drawHex(Vec2f draw_pos, float val)
{
    int hexCount = 4;
    int segments = 6;
    
    for (int i = 0; i < FILTER_SIZE; i++) {
        float a = i * 1.0 / FILTER_SIZE;
        for (int j = 0; j < hexCount; j++) {
            gl::color((i / FILTER_SIZE) * 0.25, 0.1 * j * val, 1.0 - val * 0.25 * j, a * baseAlpha);
            gl::drawStrokedCircle( draw_pos, prevTileSize[i] - (hexCount - j) * mConfig->tileBorderSpacing, segments );
        }
    }
}

// Create and draw a star tile
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
        float x = draw_pos.x + tileSize * cos(angle);
        float y = draw_pos.y + tileSize * sin(angle);

        lines[lineIdx] = x; lines[lineIdx + 1] = y;
        lines[lineIdx + 2] = draw_pos.x; lines[lineIdx + 3] = draw_pos.y;        
    }
    
    drawLines(lines, colors, LINE_COUNT);
}

// Draw active tile highlight
inline void Tile::drawActive(Vec2f draw_pos, float val) {
	gl::color(1, 1, 1, 0.5 * baseAlpha);
    gl::drawStrokedCircle( draw_pos, mConfig->tileSize, 6 );
    
    gl::color (0.6, val, 0.8, 0.4 * baseAlpha);
    gl::drawSolidCircle( draw_pos, mConfig->tileSize, 6 );    
}

// Draw selected tile highlight
inline void Tile::drawSelected(Vec2f draw_pos, float val) {
    gl::color (1.0, 0.2, 0.8, 0.40 * baseAlpha);
    gl::drawSolidCircle( draw_pos, mConfig->tileSize, 6 );        
}
