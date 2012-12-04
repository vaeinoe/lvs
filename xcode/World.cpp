/*
 *  World.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "World.h"

using namespace ci;

void World::setup( Configuration *config, const Vec2i newSize )
{
    mConfig = config;
    size = new Vec2i( newSize.x, newSize.y );
    solverTimer = mConfig->solverDelayFrames;
    selectedTile = NULL;
    
    reset();
}

void World::reset() {
    tiles.clear();
    tiles.reserve( size->x * size->y );
    
    for (int x = 0; x < size->x; x++) {
        for (int y = 0; y < size->y; y++) {
            int type = rnd.nextInt(mConfig->numTileTypes);
            Tile *tile = new Tile();
            tile->setup(mConfig, Vec2i( x, y ), type);
            tiles.push_back(tile);
        }
    }
    
    if (resolveTiles(false) == true) {
        reset();
    }
}

void World::update( const Vec2i *mouseLoc, const float *freqData, const int dataSize )
{
    
    int size = tiles.size();
    int binsPerTile = dataSize / size;
    int n = 0;
    float dataSizef = (float)dataSize;
    
    for( vector<Tile*>::iterator t = tiles.begin(); t != tiles.end(); ++t ){
        Vec2f pos = (*t)->getScreenPositionVector();
        Vec2i dir = pos - (*mouseLoc);
        float modifier = 0;
        for (int i = 0; i < binsPerTile; i++) {
            int bin = n * binsPerTile + i;
            float mod = math<float>::clamp( freqData[ bin ] * ( bin / dataSizef ) * ( math<float>::log( ( dataSizef - (float)bin ) ) ), 0.0f, 2.0f );
            modifier += mod;
        }
//        if (n < dataSize) {
//            modifier = freqData[n];
//        }
        (*t)->update(dir.length(), modifier);
        n++;
    }
    
    solverTimer--;
    if (solverTimer == 0) {
        resolveTiles(true);
        solverTimer = mConfig->solverDelayFrames;        
    }
}

inline int World::tileIndex(int x, int y) {
    return ((x * size->y) + y);
}

bool World::resolveTiles(const bool act) {
    bool hits = false;
    for (int x = 0; x < size->x; x++) {
        for (int y = 1; y < size->y - 1; y++) {
            bool result = resolveTile(x,y,act);
            if (result == true) {
                hits = true;
            }
        }
    }
    return hits;
}

bool World::resolveTile(int x, int y, bool act) {
    int idx = tileIndex(x, y);
    int idx2 = tileIndex(x, y - 1);
    int idx3 = tileIndex(x, y + 1);
    
    if (tiles[idx]->type != -1 &&
        tiles[idx2]->type == tiles[idx]->type &&
        tiles[idx3]->type == tiles[idx]->type) {
        
        if (act) {
            int score = 0;
            score += tiles[idx]->kill();
            score += tiles[idx2]->kill();
            score += tiles[idx3]->kill();
            mConfig->player->addScore(score);
        }
        
        return true;
    }
    
    return false;
}

void World::draw()
{
    for( vector<Tile*>::iterator t = tiles.begin(); t != tiles.end(); ++t ){
        (*t)->draw();
    }
}

void World::selectTile( const Vec2i mouseLoc ) {
    for( vector<Tile*>::iterator t = tiles.begin(); t != tiles.end(); ++t ){
        Vec2f pos = (*t)->getScreenPositionVector();
        Vec2i dir = pos - mouseLoc;
        
        bool hit = dir.length() < (0.9 * mConfig->tileSize);
        if (hit) {
            
            if ( selectedTile ) {
                if ( selectedTile != &**t ) {
                    selectedTile->toggleSelected();
                    if (areNeighbours(&**t, selectedTile) &&
                        selectedTile->dead == false &&
                        (*t)->dead == false) {
                        swapTiles(&**t, selectedTile);
                        selectedTile = NULL;
                    }
                    else {
                        (*t)->toggleSelected();
                        selectedTile = &**t;
                    }
                }
                else {
                    (*t)->toggleSelected();
                    selectedTile = NULL;
                }
            }
            else {
                (*t)->toggleSelected();
                selectedTile = &**t;
            }
            
            return;
        }
    }
    
    if (selectedTile) {
        selectedTile->toggleSelected();
        selectedTile = NULL;
    }
}

bool World::areNeighbours ( Tile *tile1, Tile *tile2 ) {
    // Tiles are neighbours if x value is the same and difference in y < 3
    if (tile1->pos->x == tile2->pos->x) return true;
    
    // Otherwise with right tile r and left tile l: the tiles are neighbours if l.x = r.x - 1 and l.y = r.y - 1 or l.y = r.y + 1
    Tile *l;
    Tile *r;
    if (tile1->pos->x < tile2->pos->x) {
        l = tile1;
        r = tile2;
    }
    else {
        l = tile2;
        r = tile1;
    }
    
    if ((l->pos->x == r->pos->x - 1) &&
       ((l->pos->y == r->pos->y - 1) ||
        (l->pos->y == r->pos->y + 1))) return true;
    
    return false;
}

// Swaps two tiles
void World::swapTiles( Tile *tile1, Tile *tile2 ) {
    Vec2i pos1, pos2;
    pos1.x = tile1->pos->x;
    pos1.y = tile1->pos->y;
    pos2.x = tile2->pos->x;
    pos2.y = tile2->pos->y;
    
    Tile *tileTmp = tile1;
    
    tiles[tileIndex(pos1.x, pos1.y)] = tile2;
    tiles[tileIndex(pos2.x, pos2.y)] = tileTmp;
    
    tile1->moveTo(pos2);
    tile2->moveTo(pos1);
}

void World::shutdown()
{
    tiles.clear();
}
