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

using namespace ci;
using namespace ci::app;
using namespace std;

#define NUM_ANGLES 32

#define PLAYFIELD_W 950
#define PLAYFIELD_H 650

#define MAIN_VOLUME 0.8
#define ANALYSIS_GAIN 1.0

#define TILE_TYPES 4
#define NUMTRACKS 6

#define MOVE_TIME_SEC 0.75
#define FADE_TIME_SEC 2.0
#define GROW_TIME_SEC 6.0

