/*
 *  World.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "Configuration.h"
#include "Common.h"
#include "Tile.h"

#define NUM_ANGLES 32

class Configuration;
class Tile;

class World {
public:
	void setup( Configuration *config, const Vec2i newSize );
	void update( const Vec2i *mouseLoc, const float *freqData, const int dataSize );
    void reset();
	void draw();
	void shutdown();
    void selectTile( const Vec2i mouseLoc );
    void addCirclePoly( const Vec2f &center, const float radius, const int numSegments, const ColorA &color );
private:
    Configuration *mConfig;
    Vec2i *size;
    std::vector<Tile*> tiles;
    Tile *selectedTile;
    
    void precalc();
    vector< Vec2f > precalcAngles[NUM_ANGLES];

    Rand rnd;

    Vec2i neighbourCoord ( Vec2i pos, Vec2i dir );
    bool areNeighbours ( Tile *tile1, Tile *tile2 );
    void swapTiles( Tile *tile1, Tile *tile2 );
    bool resolveTiles(const bool act);
    bool resolveTile(int x, int y, bool act);
    int tileIndex(int x, int y);

    vector< GLfloat > tileVerts;
    vector< GLfloat > tileColors;
    
    int solverTimer;
};
