/*
 *  Tile.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */
#pragma once
#include "Common.h"

class Tile {
public:
    void setup( Configuration *config, const Vec2i pos, int type);
    void update( const float dist, const float modifier );
    void draw();
    void toggleSelected();
    Vec2f getScreenPositionVector();
    Vec2i *pos;

    void moveTo( const Vec2i newPos );
private:
    Configuration *mConfig;
    
    const static int FILTER_SIZE = 16;
    float prevTileSize[FILTER_SIZE];
    
    float tileSize;
    int type;
    bool active;
    bool selected;
    
    void drawHex(Vec2f pos, float val);
    void drawStar(Vec2f pos, float val);
    void drawGram(Vec2f draw_pos, float val);

    void drawSelected(Vec2f pos, float val);
    void drawActive(Vec2f pos, float val);
    
    bool moving;
};