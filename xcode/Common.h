/*
 *  Common.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/27/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#pragma once
#include "Configuration.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/Io.h"
#include "cinder/audio/Output.h"
#include "cinder/CinderMath.h"
#include "cinder/Rand.h"
#include "ciUI.h"
#include "KissFFT.h"

inline float fast_sin(float x)
{
    const float B = 4 / pi;
    const float C = -4 / (pi * pi);
    
    return -(B * x + C * x * ((x < 0) ? -x : x));
} 
