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
#include "cinder/Thread.h"
#include "cinder/gl/TextureFont.h"
#include "ciUI.h"
#include "KissFFT.h"

inline float fast_sin(float x)
{
    const float B = 4 / pi;
    const float C = -4 / (pi * pi);
    
    return -(B * x + C * x * ((x < 0) ? -x : x));
}

inline void drawLines( const GLfloat verts[], const GLfloat colors[], const int numLines )
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );

	glColorPointer( 4, GL_FLOAT, 0, colors );
	glVertexPointer( 2, GL_FLOAT, 0, verts );
	glDrawArrays( GL_LINES, 0, numLines * 2 );

	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

