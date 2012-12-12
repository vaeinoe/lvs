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
    int rndTileType();
private:
    Configuration *mConfig;
    Vec2i *size;
    std::vector<Tile*> tiles;
    Tile *selectedTile;
    
    Rand rnd;

    Vec2i neighbourCoord ( Vec2i pos, Vec2i dir );
    bool areNeighbours ( Tile *tile1, Tile *tile2 );
    void swapTiles( Tile *tile1, Tile *tile2 );
    bool resolveTiles(const bool act);
    bool resolveTile(int x, int y, bool act);

    int tileIndex(int x, int y);
    int tileIndex(Vec2i pos);
        
    int solverTimer;
};
