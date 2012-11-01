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
    void setup( const Vec2i pos, const float sz, int type);
    void update( const float dist, const float modifier );
    void draw();
    void toggleSelected();
    Vec2f getScreenPositionVector();
    Vec2i *pos;
private:
    float baseTileSize;
    float prevTileSize[4];
    float tileSize;
    int type;
    bool active;
    bool selected;
    void drawHex(Vec2f pos);
    void drawStar(Vec2f pos);
    void drawSelected(Vec2f pos);
};