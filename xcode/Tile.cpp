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
    
    for (int i = 0; i < FILTER_SIZE; i++) {
        prevTileSize[i] = tileSize;
    }
    active = false;
    selected = false;
    type = newType;
    
    moving = false;
}

// Activates / deactivates selected hex
void Tile::toggleSelected() {
    selected = !selected;
}

// Starts moving the tile to another position
void Tile::moveTo( Vec2i newPos ) {
    pos->x = newPos.x;
    pos->y = newPos.y;
    
    moving = true;
}

void Tile::update( const float dist, const float modifier )
{
    // Activate if cursor is over the hex
    active = (dist < (0.9 * mConfig->tileSize)) ? true : false;

    // Shift averages
    for (int i = 1; i < FILTER_SIZE; i++) {
        prevTileSize[i - 1] = prevTileSize[i];
    }
    
    // Init the new tile size
    prevTileSize[FILTER_SIZE - 1] = math<float>::clamp(mConfig->tileUpperLimit * (modifier * 16), (mConfig->tileSize / 4), mConfig->tileUpperLimit);
    
    // Calculate moving average
    float sum = 0;
    for (int i = 0; i < FILTER_SIZE; i++) {
        sum += prevTileSize[i];
    }
    tileSize = (sum / FILTER_SIZE);
}

// Returns the screen position of this hex
Vec2f Tile::getScreenPositionVector() {
    float x = mConfig->padding + (mConfig->tileGrid / 2) + pos->x * (1.5 * mConfig->tileGrid);
    float y = mConfig->padding + (mConfig->tileGrid / 2) + pos->y * (0.43 * mConfig->tileGrid);
    if (pos->y % 2 != 0) { x += (0.74 * mConfig->tileGrid); }
    
    return Vec2f(x, y);
}

// Draws the tile
void Tile::draw()
{
    Vec2f draw_pos = getScreenPositionVector();
    float val = sin((getElapsedSeconds() * 2 + sin(draw_pos.x) + cos(draw_pos.y))) * 0.2f + 0.5f;

    float lightness = sin(getElapsedSeconds() / 10);
	gl::color(0, 0, lightness * 0.4, 0.75);
    gl::drawSolidCircle( draw_pos, mConfig->tileSize, 6 );

    gl::color (0.8, 0.5, 1.0, 0.25);
    gl::drawStrokedCircle( draw_pos, mConfig->tileSize, 6 );
    
    gl::color (1.0, 1.0, 1.0, val);
    
    float a = 0.3;
    for (int i = 1; i < 3; i++) {
        gl::color (1.0, 1.0, 1.0, a);
        gl::drawStrokedCircle( draw_pos, tileSize + (3 * i) + 2, 6 );
        a = a * 0.5;
    }

    if (type == 0) drawHex(draw_pos, val);
    else if (type == 1) drawStar(draw_pos, val);

    if (active) drawActive(draw_pos, val);    
    if (selected) drawSelected(draw_pos, val);
}

// Draw a hex tile
inline void Tile::drawHex(Vec2f draw_pos, float val)
{
    for (int i = 0; i < FILTER_SIZE; i++) {
        float a = i * 1.0 / FILTER_SIZE;
        for (int j = 0; j < 4; j++) {
            gl::color (0.0, (0.1 * j) * val, 1.0 - val * (0.25 * j), a);
            gl::drawStrokedCircle( draw_pos, prevTileSize[i] - (4 - j) * mConfig->tileBorderSpacing, 6 );
        }
    }
}

// Draw a star tile
inline void Tile::drawStar(Vec2f draw_pos, float val)
{
    for (int i = 0; i < 32; i++) {
        float a = (i % 2 == 0) ? 0.5 : 0.25;
        gl::color (1.0, 0.6, 1.0, a);
        
        float angle = (i / 32.0f) * (2 * pi);
        float x = draw_pos.x + tileSize * cos(angle);
        float y = draw_pos.y + tileSize * sin(angle);
        gl::drawLine(Vec2f( x, y ), Vec2f( draw_pos.x, draw_pos.y ));
    }
}

// Draw active tile highlight
inline void Tile::drawActive(Vec2f draw_pos, float val) {
	gl::color(1, 1, 1, 0.5);
    gl::drawStrokedCircle( draw_pos, mConfig->tileSize, 6 );
    
    gl::color (0.6, 0.0, 0.8, 0.4);
    gl::drawSolidCircle( draw_pos, mConfig->tileSize, 6 );    
}

// Draw selected tile highlight
inline void Tile::drawSelected(Vec2f draw_pos, float val) {
    gl::color (1.0, 0.2, 0.8, 0.40);
    gl::drawSolidCircle( draw_pos, mConfig->tileSize, 6 );        
}
