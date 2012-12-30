/*
 *  Tile.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */
#pragma once
#include "Configuration.h"
#include "FadeObserver.h"

class Configuration;
class Fader;
class FaderPack;

class Tile : public FadeObserver {
public:
    void setup( Configuration *config, const Vec2i pos, int type, bool grow );
    void update( const bool floating, const float dist, const float modifier );
    void draw();
    void toggleSelected();
    void setSurrounding(bool value);
    void shutdown();
    
    Vec2f getScreenPositionVector();
    Vec2i *pos;

    void moveTo( const Vec2i newPos );
    void shrink();
    
    void pause();
    void resume();
    bool paused;
    
    int kill(int mult = 1);
    bool selectable();
    
    void onFadeEnd(int typeId);
    
    int type;
private:
    Configuration *mConfig;
    bool dead;
    Vec2f getScreenPositionVector(Vec2i loc, bool shiftOrigin = true);
    
    const static int LINE_COUNT = 32;
    
    const static int FILTER_SIZE = 16;
    float prevTileSize[FILTER_SIZE];
    
    float tileSize;
    float baseTileSize;
    
    bool active;
    bool selected;
    bool surrounding;
    
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
    void drawSurrounding(Vec2f pos, float val);
    void drawFinished();
    
    void drawDropShadow();
    
    Vec2f drawPos;
    
    bool finished;
    double finishAlpha;
    
    Fader *fadeFader;
    double baseAlpha;
    double fading;
    
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
    
    bool shrinking;
};
