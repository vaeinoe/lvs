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
    rnd.seed(time(0));
    
    mConfig = config;
    size = new Vec2i( newSize.x, newSize.y );
    solverTimer = mConfig->solverDelayFrames;
    selectedTile = NULL;
    
    reset();
}

int World::rndTileType() {
    return rnd.nextInt(mConfig->numTileTypes);
}

void World::reset() {
    tiles.clear();
    tiles.reserve( size->x * size->y );
    
    for (int x = 0; x < size->x; x++) {
        for (int y = 0; y < size->y; y++) {
            int type = rndTileType();
            Tile *tile = new Tile();
            tile->setup(mConfig, Vec2i( x, y ), type, false);
            tiles.push_back(tile);
        }
    }
    
    // If there's at least one match already, try another tile combination
    if (resolveTiles(false) == true) { reset(); }
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
            // How does this I don't even
            float mod = math<float>::clamp( freqData[ bin ] * ( bin / dataSizef ) * ( math<float>::log( ( dataSizef - (float)bin ) ) ), 0.0f, 2.0f );
            modifier += mod;
        }
        (*t)->update(dir.length(), modifier);
        n++;
    }
    
    solverTimer--;
    if (solverTimer == 0) {
        resolveTiles(true);
        solverTimer = mConfig->solverDelayFrames;        
    }
}

// Mimic 2d array indexing
inline int World::tileIndex(int x, int y) { return ((x * size->y) + y); }
inline int World::tileIndex(Vec2i pos) { return ((pos.x * size->y) + pos.y); }

// Returns the coordinate of tile's immediate neighbour to direction dir (lengths ignored)
inline Vec2i World::neighbourCoord(Vec2i pos, Vec2i dir)
{
    // N + S same for both alignments
    if (dir.x == 0) {
        if      (dir.y < 0) { return Vec2i(pos.x, pos.y - 2); }
        else if (dir.y > 0) { return Vec2i(pos.x, pos.y + 2); }
        else                 { return Vec2i(pos.x, pos.y);     }
    }

    Vec2i ret = Vec2i(0,0);
    
    if      (dir.y < 0) { ret.y = pos.y - 1; }
    else if (dir.y > 0) { ret.y = pos.y + 1; }
    else                 { return Vec2i(pos.x, pos.y);  } // No neighbouring tiles left & right
    
    if      (dir.x > 0) { ret.x = pos.x;     }
    else                 { ret.x = pos.x - 1; }
    
    // Even & odd rows handle this differently
    if (pos.y % 2 == 1) { ret.x += 1; }

    return ret;
    
        /* EVEN
           x   y
           NE: +0  -1
           NW: -1, -1
           SE: +0, +1
           SW: -1, +1 */

        /* ODD
           x   y
           NE: +1  -1
           NW: +0  -1
           SE: +1  +1
           SW: +0  +1 */
}

// Run resolver on all tiles
bool World::resolveTiles(const bool act) {
    bool hits = false;
    for (int x = 0; x < size->x; x++) {
        for (int y = 2; y < size->y - 2; y++) {
            bool result = resolveTile(x, y, act);
            if (result == true) hits = true;
        }
    }
    return hits;
}

// Resolve the area around tile x, y - if act false,
// doesn't kill tiles or update score
bool World::resolveTile(int x, int y, bool act) {
    Vec2i pos = Vec2i(x, y);
    int tileIdx = tileIndex(pos);

    if (!tiles[tileIdx]->selectable()) { return false; }

    // These are enough for hit
    int reqTiles[] = {
        tileIndex(neighbourCoord(pos, Vec2i(0,  1))),
        tileIndex(neighbourCoord(pos, Vec2i(0, -1)))
    };
    
    // These give extra points
    int extraTiles[] = {
        tileIndex(neighbourCoord(pos, Vec2i(1,  1))),
        tileIndex(neighbourCoord(pos, Vec2i(1, -1))),
        tileIndex(neighbourCoord(pos, Vec2i(-1,  1))),
        tileIndex(neighbourCoord(pos, Vec2i(-1, -1)))
    };
    
    int type = tiles[tileIdx]->type;
    
    int reqHit = true;
    for (int i = 0; i < 2; i++) {
        if (reqTiles[i] >= tiles.size() || !tiles[reqTiles[i]]->selectable() || tiles[reqTiles[i]]->type != type) {
            reqHit = false;
            break;
        }
    }

    if (!reqHit) return false;

    int extraHit = true;
    for (int i = 0; i < 4; i++) {
        if (extraTiles[i] >= tiles.size() || !tiles[extraTiles[i]]->selectable() || tiles[extraTiles[i]]->type != type) {
            extraHit = false;
            break;
        }
    }

    if (act) {
        mConfig->audio->playTileFx(0, 1);
        tiles[tileIdx]->kill();
        
        int mult = 2;
        if (extraHit) mult = 3;
        
        if (reqHit) {
            for (int i = 0; i < 2; i++) {
                tiles[reqTiles[i]]->kill(mult);
            }
        }
        if (extraHit) {
            for (int i = 0; i < 4; i++) {
                tiles[extraTiles[i]]->kill(mult);
            }
        }
    }

    return true;
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
        
        // Harrison-Stetson approximation of hit area
        bool hit = dir.length() < (0.9 * mConfig->tileSize);
        if (hit && (*t)->selectable() ) {
            
            if ( selectedTile ) {
                if ( selectedTile != &**t ) {
                    selectedTile->toggleSelected();
                    if (areNeighbours(&**t, selectedTile) && selectedTile->selectable() && (*t)->selectable()) {
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
    
    // Still a tile selected but it wasn't this one? Deselect!
    if (selectedTile) {
        selectedTile->toggleSelected();
        selectedTile = NULL;
    }
}

// Returns true if tile1 != tile2 and tiles are neighbours on the grid
bool World::areNeighbours ( Tile *tile1, Tile *tile2 ) {
    int x1 = tile1->pos->x;
    int x2 = tile2->pos->x;
    int y1 = tile1->pos->y;
    int y2 = tile2->pos->y;

    // A tile is not its own neighbour
    if (x1 == x2 && y1 == y2) return false;
    
    int y_dist = abs(y1 - y2);
    int x_dist = abs(x1 - x2);

    // Tiles are not neighbours if they have same y (interleaved y axis)
    // or are too far from each other in either axis
    if (y_dist == 0 || y_dist > 2 || x_dist > 1)  return false;

    // Tiles are always neighbours if x value is the same and difference in y < 3
    if (x1 == x2) return true;

    // Rules for even / odd tiles
    if (y_dist == 1) {
        bool even_y = y1 % 2 == 1;
        if (even_y && x1 - x2 == -1) return true;
        if (!even_y && x1 - x2 == 1) return true;
    }
    
    // No match
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

    mConfig->player->addScore(-1, tile1->type);
    mConfig->player->addScore(-1, tile2->type);
}

void World::shutdown()
{
    tiles.clear();
}
