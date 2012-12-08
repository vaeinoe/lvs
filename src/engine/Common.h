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
#include "cinder/Timer.h"
#include "cinder/Utilities.h"
#include "cinder/gl/TextureFont.h"
#include "../ciUI/ciUI.h"
#include "KissFFT.h"

#define MOVE_TIME_SEC 1.0
#define FADE_TIME_SEC 2.0

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

/*
inline void addCirclePoly( const Vec2f &center, float r, int numSegments)
{
    if( numSegments <= 0 ) {
		numSegments = (int)math<double>::floor( r * M_PI * 2 );
	}
	if( numSegments < 2 ) numSegments = 2;

	float theta = 2 * M_PI / float(numSegments);
	float c = cosf(theta); //precalculate the sine and cosine
	float s = sinf(theta);
	float t;
    
	float x = r; //we start at angle = 0
	float y = 0;

    vector< GLfloat > verts;
    
    float sX = x + center.x;
    float sY = y + center.y;

	for(int i = 0; i < numSegments; i++)
	{
		verts.push_back(x + center.x);
        verts.push_back(y + center.y);
        
		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
        
        if (i > 0) {
            verts.push_back(x + center.x);
            verts.push_back(y + center.y);
        }
	}
    
    verts.push_back(sX);
    verts.push_back(sY);

    glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 2, GL_FLOAT, 0, &verts[0] );
	glDrawArrays( GL_LINES, 0, verts.size() );
	glDisableClientState( GL_VERTEX_ARRAY );
}
*/

