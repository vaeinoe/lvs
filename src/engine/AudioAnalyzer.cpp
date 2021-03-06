//
//  AudioAnalyzer.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/8/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "AudioAnalyzer.h"

void AudioAnalyzer::setup(Configuration *config)
{
    mConfig = config;
    audioSize = 0;
    audioFloat = NULL;

    for (int i = 0; i < 5; i++) {
        PolyLine<Vec2f> line;
        line.push_back(Vec2f(0,0));
        line.push_back(Vec2f(0,0));
        prevLines[i] = line;
    }
}

void AudioAnalyzer::update()
{
    if (audioSize > 0) {
        if ( !mFft ) { mFft = Kiss::create( audioSize ); }
        
        mFft->setData( audioFloat );

        freqData = mFft->getAmplitude();
        timeData = mFft->getData();
        dataSize = mFft->getBinSize();
        
        audioSize = 0;
    }
}

void AudioAnalyzer::draw(float scaleIn, float offsetIn)
{
    if ( mFft ) {
        glLineWidth( toPixels(2.6f) );
        
		float dataSizef = (float)dataSize;
        
		// Get dimensions
		float scale = (( (float)mConfig->fieldSize.x ) / dataSizef) * scaleIn;
		float windowHeight = (float)mConfig->fieldSize.y + offsetIn;
        
		// Use polylines to depict time and frequency domains
		PolyLine<Vec2f> freqLine;
		PolyLine<Vec2f> timeLine;
        
		// Iterate through data
		for ( int32_t i = 0; i <= dataSize; i++ ) {
            
			// Do logarithmic plotting for frequency domain
			float logSize = math<float>::log( dataSizef );
			float x = (float)( ( math<float>::log( (float)i ) / logSize ) * dataSizef );
			float y = math<float>::clamp( freqData[ i ] * ( x / dataSizef ) * ( math<float>::log( ( dataSizef - (float)i ) ) ), 0.0f, 2.0f );
            
			freqLine.push_back( Vec2f( mConfig->fieldOrigin.x + x * scale,
                                       mConfig->fieldOrigin.y + -y * ( windowHeight - 20.0f ) * 0.25f + ( windowHeight - 10.0f ) ) );
			timeLine.push_back( Vec2f( mConfig->fieldOrigin.x + (float)i * scale,
                                       mConfig->fieldOrigin.y + timeData[ i ] * ( windowHeight - 20.0f ) * 0.25f + ( windowHeight * 0.5f ) ) );
		}
        
        for (int i = 0; i < 5; i++) {
            gl::color( 0.8, 0.0, 0.3, 0.5 * (i * 1.0 / 6) );
            gl::draw(prevLines[i]);
        }
        
        for (int i = 1; i < 5; i++) {
            prevLines[i-1] = prevLines[i];
        }
        prevLines[4] = timeLine;
        
        gl::color( 0.7, 0.4, 0.8, 0.6);
		// gl::draw( freqLine );
		gl::draw( timeLine );
        
        glLineWidth( toPixels(1.7f) );
    }
    
}

void AudioAnalyzer::updateData ( const void * data, int byteCount, int playbackrate )
{
    audioSize = byteCount / 2;
    sampleRate = playbackrate;
    audioFloat = (float *)data;
}
