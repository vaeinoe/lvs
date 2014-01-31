/*
 *  World.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#include "World.h"
#include "Player.h"
#include "Tile.h"
#include "LVSEngine.h"
#include "TileLevel.h"

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


// Clears and reinits all tiles
void World::reset() {
    for( vector<Tile*>::iterator t = tiles.begin(); t != tiles.end(); ++t ){
        (*t)->shutdown();
        delete (*t);
    }
    
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

void World::pause()
{
    for( vector<Tile*>::iterator t = tiles.begin(); t != tiles.end(); ++t ){
        (*t)->pause();
    }
}

void World::resume()
{
    for( vector<Tile*>::iterator t = tiles.begin(); t != tiles.end(); ++t ){
        (*t)->resume();
    }
}

// Shrink and fade out all tiles in the end of the game
void World::shrink()
{
    for( vector<Tile*>::iterator t = tiles.begin(); t != tiles.end(); ++t ){
        (*t)->shrink();
    }
}


// Update all tiles' size modifiers and active status
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
        (*t)->update(!mConfig->engine->dragging, dir.length(), modifier);
        n++;
    }
    
    solverTimer--;
    if (solverTimer == 0) {
        resolveTiles(true);
        solverTimer = mConfig->solverDelayFrames;        
    }
}


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
    
    int bonusTiles[3];
    Vec2i curr = neighbourCoord(pos, Vec2i(0,  1));
    for (int i = 0; i < 3; i++) {
        curr = neighbourCoord(curr, Vec2i(0,  1));
        bonusTiles[i] = tileIndex(curr);
    }
        
    // These give extra points
    int hextraTiles[] = {
        tileIndex(neighbourCoord(pos, Vec2i(1,  1))),
        tileIndex(neighbourCoord(pos, Vec2i(1, -1))),
        tileIndex(neighbourCoord(pos, Vec2i(-1,  1))),
        tileIndex(neighbourCoord(pos, Vec2i(-1, -1)))
    };
    
    int type = tiles[tileIdx]->type;
    
    int reqHit = true;
    for (int i = 0; i < 2; i++) {
        if (reqTiles[i] >= tiles.size() || reqTiles[i] < 0 ||
            !tiles[reqTiles[i]]->selectable() || tiles[reqTiles[i]]->type != type) {
                reqHit = false;
                break;
        }
    }

    if (!reqHit) return false;

    int hextraHit = true;
    for (int i = 0; i < 4; i++) {
        if (hextraTiles[i] >= tiles.size() || hextraTiles[i] < 0 ||
            !tiles[hextraTiles[i]]->selectable() || tiles[hextraTiles[i]]->type != type) {
                hextraHit = false;
                break;
        }
    }

    if (act) {
        tiles[tileIdx]->kill();
        
        int mult = 2;
        if (hextraHit) mult = 4;
        
        if (reqHit) {
            for (int i = 0; i < 2; i++) {
                tiles[reqTiles[i]]->kill(mult);
            }
        }
        
        if (hextraHit) {
            for (int i = 0; i < 4; i++) {
                tiles[hextraTiles[i]]->kill(mult);
            }
        }
        else { // Resolve bonus tiles if no hex hit
            for (int i = 0; i < 3; i++) {
                if (bonusTiles[i] < tiles.size() && bonusTiles[i] > 0 &&
                    tiles[bonusTiles[i]]->selectable() && tiles[bonusTiles[i]]->type == type) {
                    tiles[bonusTiles[i]]->kill(mult + (2 * (i+1)));
                }
                else {
                    break;
                }
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

// Returns a tile hit under the cursor
inline Tile* World::getTileAt( Vec2i mouseLoc ) {
    for( vector<Tile*>::iterator t = tiles.begin(); t != tiles.end(); ++t ){
        Vec2f pos = (*t)->getScreenPositionVector();
        Vec2i dir = pos - mouseLoc;
        if (dir.length() < (0.95 * mConfig->tileSize)) return (*t);
    }
    
    return NULL;
}

// Toggles "surround" highlight for all the neigbouring tiles of tile
inline void World::setSurrounding( Tile *tile, bool value ) {
    Vec2i pos;
    pos.x = tile->pos->x;
    pos.y = tile->pos->y;
    
    int neighbours[] = {
        tileIndex(neighbourCoord(pos, Vec2i(0,  1))),
        tileIndex(neighbourCoord(pos, Vec2i(0, -1))),
        tileIndex(neighbourCoord(pos, Vec2i(1,  1))),
        tileIndex(neighbourCoord(pos, Vec2i(1, -1))),
        tileIndex(neighbourCoord(pos, Vec2i(-1,  1))),
        tileIndex(neighbourCoord(pos, Vec2i(-1, -1)))
    };

    for (int i = 0; i < 6; i++) {
        if (neighbours[i] < tiles.size() && neighbours[i] > 0) {
            tiles[neighbours[i]]->setSurrounding(value);
        }
    }
}

// Deselect + swap if required
void World::deselectTile( const Vec2i mouseLoc ) {
    if (selectedTile) {
        selectedTile->toggleSelected();
        setSurrounding(selectedTile, false);
        
        Tile *hover = getTileAt( mouseLoc );
        if (hover && areNeighbours(selectedTile, hover) &&
            selectedTile->selectable() && hover->selectable()) {
                swapTiles(hover, selectedTile);
        }
        
        selectedTile = NULL;
    }    
}

// Selects a new tile and marks the surrounding tiles as highlighted
void World::selectTile( const Vec2i mouseLoc ) {
    if (selectedTile) {
        selectedTile->toggleSelected();
        selectedTile = NULL;
    }
    Tile *hover = getTileAt( mouseLoc );
    
    if (hover && hover->selectable()) {
        hover->toggleSelected();
        selectedTile = hover;
        setSurrounding(hover, true);
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

    mConfig->levels[tile1->type]->addScore(-1);
    mConfig->levels[tile2->type]->addScore(-1);
}

int World::rndTileType() { return rnd.nextInt(mConfig->numTileTypes); }

inline int World::tileIndex(int x, int y) {
    if (x < 0 || y < 0) return -1;
    if (x >= size->x || y >= size->y) return -1;
    return ((x * size->y) + y);
}

inline int World::tileIndex(Vec2i pos) {
    if (pos.x < 0 || pos.y < 0) return -1;
    if (pos.x >= size->x || pos.y >= size->y) return -1;
    return ((pos.x * size->y) + pos.y);
}

void World::shutdown() { tiles.clear(); }
