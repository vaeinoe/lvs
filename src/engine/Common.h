/*
 *  Common.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/CinderMath.h"
#include "cinder/Rand.h"
#include "cinder/Timer.h"
#include "cinder/Utilities.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/svg/SvgGl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define FONT_TYPE_SMALL  0
#define FONT_TYPE_MEDIUM 1
#define FONT_TYPE_LARGE  2

#define FONT_SIZE_SMALL  14
#define FONT_SIZE_MEDIUM 32
#define FONT_SIZE_LARGE  56

#define NUM_ANGLES 32

#define PLAYFIELD_W 1024
#define PLAYFIELD_H 768

#define MAIN_VOLUME 0.8
#define ANALYSIS_GAIN 1.0

#define TILE_TYPES 4
#define NUMTRACKS 6

#define MOVE_TIME_SEC 0.75
#define FADE_TIME_SEC 2.0
#define GROW_TIME_SEC 6.0
#define SHRINK_TIME_SEC 10.0

#define INIT_GAME_TIME 60.0
#define WARN_GAME_TIME 10.0

// Original: 50
#define LEVEL_SCORE_MODIFIER 25

#define GAME_NAME "lvs"
#define ENDGAME_STR "fin."
#define WINGAME_STR "you are victorious."
#define GAME_START_STR "commence."

#define GAME_FONT "Helvetica Bold"

#define HIGHSCORE_KEY "30DFB6B8LONG1D8D4CAT929A3CFIS4D0726LONGCC05E7"
