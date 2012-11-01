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
#include "cinder/audio/Io.h"
#include "cinder/audio/Output.h"
#include "cinder/CinderMath.h"
#include "cinder/Rand.h"
#include "ciUI.h"
#include "KissFFT.h"

const static int TOOLBAR_HEIGHT = 75;
const static float TILE_SIZE = 50.0f;
const static float TILE_GRID = 120.0f;
const static float TILE_BORDER_SPACING = 10.0f;
const static int WORLD_WIDTH = 5;
const static int WORLD_HEIGHT = 11;
const static float PADDING = 10.0f;

inline float fast_sin(float x)
{
    const float B = 4 / pi;
    const float C = -4 / (pi * pi);
    
    return -(B * x + C * x * ((x < 0) ? -x : x));
} 
