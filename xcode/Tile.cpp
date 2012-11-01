/*
 *  Tile.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#include "Tile.h"

void Tile::setup( const Vec2i newPos, const float newSize, int newType)
{
    pos = new Vec2i( newPos.x, newPos.y );
    baseTileSize = newSize;
    tileSize = baseTileSize;
    
    for (int i = 0; i < 5; i++) {
        prevTileSize[i] = tileSize;        
    }
    active = false;
    selected = false;
    
    type = newType;
}

void Tile::toggleSelected() {
    if ( selected == true ) {
        selected = false;
    }
    else {
        selected = true;
    }
}

void Tile::update( const float dist, const float modifier )
{
    if (dist < (0.9 * baseTileSize)) {
        active = true;
    }
    else {
        active = false;
    }

    for (int i = 1; i < 5; i++) {
        prevTileSize[i] = prevTileSize[i - 1];
    }
    
    prevTileSize[0] = tileSize;
    tileSize = (0.1 * prevTileSize[0]) + 
               (0.1 * prevTileSize[1]) +  
               (0.1 * prevTileSize[2]) +  
               (0.1 * prevTileSize[3]) +  
               (0.1 * prevTileSize[4]) +  
               (0.5 * (math<float>::clamp(baseTileSize - (baseTileSize * 20 * modifier), 0.0f, 50.0f) ));
}

Vec2f Tile::getScreenPositionVector() {
    float x = PADDING + (TILE_GRID / 2) + pos->x * (1.5 * TILE_GRID);
    float y = PADDING + (TILE_GRID / 2) + pos->y * (0.43 * TILE_GRID);
    if (pos->y % 2 != 0) { x += (0.74 * TILE_GRID); }
    
    return Vec2f(x, y);
}

void Tile::draw()
{
    Vec2f draw_pos = getScreenPositionVector();
    if (type == 0) {
        drawHex(draw_pos);
    }
    else {
        drawStar(draw_pos);
    }
}

inline void Tile::drawHex(Vec2f draw_pos)
{
    float val = sin((getElapsedSeconds() * 2 + sin(draw_pos.x) + cos(draw_pos.y))) * 0.2f + 0.5f;
    
    gl::color (0.0, 0.2 * val, 1.0 - val * 0.5);
    gl::drawStrokedCircle( draw_pos, tileSize - 4 * TILE_BORDER_SPACING, 6 );
    gl::color (0.0, 0.6 * val, 1.0 - val);
    gl::drawStrokedCircle( draw_pos, tileSize - 3 * TILE_BORDER_SPACING, 6 );
    gl::color (0.0, 0.6 * val, 1.0 - val);
    gl::drawStrokedCircle( draw_pos, tileSize - 2 * TILE_BORDER_SPACING, 6 );
    gl::color (0.0, val, 1.0 - 0.2 * val);
    gl::drawStrokedCircle( draw_pos, tileSize - TILE_BORDER_SPACING, 6 );
    gl::color (1.0, 1.0, 1.0);
    gl::drawStrokedCircle( draw_pos, baseTileSize, 6 );
    
    gl::color (0.0, 0.2 * val, 1.0 - val * 0.5, 0.5);
    gl::drawStrokedCircle( draw_pos, prevTileSize[0] - 4 * TILE_BORDER_SPACING, 6 );
    gl::color (0.0, 0.6 * val, 1.0 - val, 0.5);
    gl::drawStrokedCircle( draw_pos, prevTileSize[0] - 3 * TILE_BORDER_SPACING, 6 );
    gl::color (0.0, 0.6 * val, 1.0 - val, 0.5);
    gl::drawStrokedCircle( draw_pos, prevTileSize[0] - 2 * TILE_BORDER_SPACING, 6 );
    gl::color (0.0, val, 1.0 - 0.2 * val, 0.5);
    gl::drawStrokedCircle( draw_pos, prevTileSize[0] - TILE_BORDER_SPACING, 6 );
    
    float a = 0.3;
    for (int i = 1; i < 3; i++) {
        gl::color (1.0, 1.0, 1.0, a);
        gl::drawStrokedCircle( draw_pos, tileSize + (3 * i) + 2, 6 );
        a = a * 0.5;
    }
    
    if (active) {
        gl::color (0.4, 0.0, 0.8, 0.4);        
        gl::drawSolidCircle( draw_pos, baseTileSize, 6 );
    }
    
    if (selected) {
        drawSelected(draw_pos);
    }
}

inline void Tile::drawSelected(Vec2f draw_pos) {
    gl::color (1.0, 0.6, 1.0, 0.40);
    gl::drawSolidCircle( draw_pos, baseTileSize, 6 );        
    gl::drawSolidCircle( draw_pos, baseTileSize - 8, 6 );    
    gl::drawSolidCircle( draw_pos, baseTileSize - 16, 6 );    
}

inline void Tile::drawStar(Vec2f draw_pos)
{
    float val = sin((getElapsedSeconds() * 2 + sin(draw_pos.x) + cos(draw_pos.y))) * 0.2f + 0.5f;
    
    /*
    gl::color (1.0, 0.2 * val, 1.0 - val * 0.5);
    gl::drawStrokedCircle( draw_pos, tileSize - 4 * TILE_BORDER_SPACING, 6 );
    gl::color (1.0, 0.6 * val, 1.0 - val);
    gl::drawStrokedCircle( draw_pos, tileSize - 3 * TILE_BORDER_SPACING, 6 );
    gl::color (1.0, 0.6 * val, 1.0 - val);
    gl::drawStrokedCircle( draw_pos, tileSize - 2 * TILE_BORDER_SPACING, 6 );
    gl::color (1.0, val, 1.0 - 0.2 * val);
    gl::drawStrokedCircle( draw_pos, tileSize - TILE_BORDER_SPACING, 6 ); */
    gl::color (1.0, 1.0, 1.0);
    gl::drawStrokedCircle( draw_pos, baseTileSize, 6 );

    /*
    gl::color (1.0, 0.2 * val, 1.0 - val * 0.5, 0.5);
    gl::drawStrokedCircle( draw_pos, prevTileSize[0] - 4 * TILE_BORDER_SPACING, 6 );
    gl::color (1.0, 0.6 * val, 1.0 - val, 0.5);
    gl::drawStrokedCircle( draw_pos, prevTileSize[0] - 3 * TILE_BORDER_SPACING, 6 );
    gl::color (1.0, 0.6 * val, 1.0 - val, 0.5);
    gl::drawStrokedCircle( draw_pos, prevTileSize[0] - 2 * TILE_BORDER_SPACING, 6 );
    gl::color (1.0, val, 1.0 - 0.2 * val, 0.5);
    gl::drawStrokedCircle( draw_pos, prevTileSize[0] - TILE_BORDER_SPACING, 6 ); */

    gl::color (1.0, 1.0, 1.0, val);

    float crossLength = baseTileSize;
    float diagLength = baseTileSize * 0.66;
    gl::drawLine(Vec2f( draw_pos.x - diagLength, draw_pos.y - diagLength ), 
                 Vec2f( draw_pos.x + diagLength, draw_pos.y + diagLength ));

    gl::drawLine(Vec2f( draw_pos.x - diagLength, draw_pos.y + diagLength ), 
                 Vec2f( draw_pos.x + diagLength, draw_pos.y - diagLength ));

    gl::drawLine(Vec2f( draw_pos.x + crossLength, draw_pos.y ), 
                 Vec2f( draw_pos.x - crossLength, draw_pos.y ));

    gl::drawLine(Vec2f( draw_pos.x, draw_pos.y + crossLength ), 
                 Vec2f( draw_pos.x, draw_pos.y - crossLength ));

    float a = 0.3;
    for (int i = 1; i < 3; i++) {
        gl::color (1.0, 1.0, 1.0, a);
        gl::drawStrokedCircle( draw_pos, tileSize + (3 * i) + 2, 6 );
        a = a * 0.5;
    }
    
    if (active) {
        gl::color (0.6, 0.0, 0.8, 0.4);        
        gl::drawSolidCircle( draw_pos, baseTileSize, 6 );
    }

    if (selected) {
        drawSelected(draw_pos);
    }    
}
