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
#include "Configuration.h"
#include "Fader.h"
#include "FaderPack.h"
#include "FadeObserver.h"

class Configuration;

class Tile : public FadeObserver {
public:
    void setup( Configuration *config, const Vec2i pos, int type, bool grow );
    void update( const float dist, const float modifier );
    void draw();
    void toggleSelected();
    Vec2f getScreenPositionVector();
    Vec2i *pos;

    void moveTo( const Vec2i newPos );
    int kill(int mult = 1);
    bool selectable();
    
    void onFadeEnd(int typeId);
    
    int type;
private:
    Configuration *mConfig;
    bool dead;
    Vec2f getScreenPositionVector(Vec2i loc);
    
    const static int LINE_COUNT = 32;
    
    const static int FILTER_SIZE = 16;
    float prevTileSize[FILTER_SIZE];
    
    float tileSize;
    float baseTileSize;
    
    bool active;
    bool selected;
    
    void regrow();
    
    void drawAlive(float lightness);
    void drawDead(float lightness);
    void drawGrow(float lightness);
    
    void drawHex(Vec2f pos, float val, int level = 0);
    void drawStar(Vec2f pos, float val, int level = 0);
    void drawGram(Vec2f draw_pos, float val, int level = 0);
    void drawPlant(Vec2f draw_pos, float val, int level = 0);
    void drawLabel(Vec2f draw_pos);

    void drawSelected(Vec2f pos, float val);
    void drawActive(Vec2f pos, float val);
    
    Vec2f drawPos;
    
    Fader *fadeFader;
    double baseAlpha;
    
    Fader *moveFader;
    double movePos;
    bool moving;
    Vec2i  moveSrcPos;
    Vec2f  moveSrcLoc;
    Vec2i  moveDestPos;
    Vec2f  moveDestLoc;

    Fader *growFader;
    double growPos;
    bool growing;
};