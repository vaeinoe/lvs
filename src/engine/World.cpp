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
    
    precalc();
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
    Vec2i pup = neighbourCoord(pos, Vec2i(0,  1));
    Vec2i pdo = neighbourCoord(pos, Vec2i(0, -1));

    int idx = tileIndex(pos.x, pos.y);
    int idx2 = tileIndex(pup.x, pup.y);
    int idx3 = tileIndex(pdo.x, pdo.y);
    
    if (tiles[idx]->selectable() &&
        tiles[idx2]->selectable() &&
        tiles[idx3]->selectable() &&
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

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
    
	glColorPointer( 4, GL_FLOAT, 0, &tileColors[0] );
	glVertexPointer( 2, GL_FLOAT, 0, &tileVerts[0] );
	glDrawArrays( GL_LINES, 0, tileVerts.size() / 2 );
    
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
    
    tileColors.clear();
    tileVerts.clear();
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

// Precalculate angle arrays for polygon drawing
void World::precalc() {
    for (int i = 2; i < NUM_ANGLES; i++) {
        for( int s = 0; s < i; s++ ) {
            float t = s / (float)i * 2.0f * 3.14159f;
            precalcAngles[i].push_back( Vec2f(math<float>::cos( t ), math<float>::sin( t )) );
        }
    }
}

// Adds a polygon to rendering queue
void World::addCirclePoly( const Vec2f &center, const float radius, int numSegments, const ColorA &color )
{
	// automatically determine the number of segments from the circumference
	if( numSegments <= 0 ) {
		numSegments = (int)math<double>::floor( radius * M_PI * 2 );
	}
	if( numSegments < 2 ) numSegments = 2;

    // first vertex
    tileVerts.push_back(center.x + precalcAngles[numSegments][0].x * radius);
    tileVerts.push_back(center.y + precalcAngles[numSegments][0].y * radius);
    tileColors.push_back(color.r); tileColors.push_back(color.g);
    tileColors.push_back(color.b); tileColors.push_back(color.a);
    
    // middle vertices
	for( int s = 1; s < numSegments; s++ ) {
        for (int i = 0; i < 2; i++) {
            tileVerts.push_back(center.x + precalcAngles[numSegments][s].x * radius);
            tileVerts.push_back(center.y + precalcAngles[numSegments][s].y * radius);
            tileColors.push_back(color.r); tileColors.push_back(color.g);
            tileColors.push_back(color.b); tileColors.push_back(color.a);
        }        
	}
    
    // drawing line loops = back to first vertex in the end
    tileVerts.push_back(center.x + precalcAngles[numSegments][0].x * radius);
    tileVerts.push_back(center.y + precalcAngles[numSegments][0].y * radius);
    tileColors.push_back(color.r); tileColors.push_back(color.g);
    tileColors.push_back(color.b); tileColors.push_back(color.a);
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
    
    mConfig->player->addScore(-1);
}

void World::shutdown()
{
    tiles.clear();
}
