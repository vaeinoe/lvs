/*
 *  Audio.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/28/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#include "Audio.h"

void Audio::setup(Configuration *config)
{
    mConfig = config;
    mAudioSource = audio::load( loadResource( "gametest1.mp3" ) );
	mTrack = audio::Output::addTrack( mAudioSource, false );
	mTrack->enablePcmBuffering( true );
	mTrack->play();
}

void Audio::update()
{
	if ( mTrack->isPlaying() && mTrack->isPcmBuffering() ) {
        
		mBuffer = mTrack->getPcmBuffer();
		if ( mBuffer && mBuffer->getInterleavedData() ) {
            
			uint32_t sampleCount = mBuffer->getInterleavedData()->mSampleCount;
			if ( sampleCount > 0 ) {
                
				if ( !mFft ) {
					mFft = Kiss::create( sampleCount );
				}

				if ( mBuffer->getInterleavedData()->mData != 0 ) {
					mFft->setData( mBuffer->getInterleavedData()->mData );
				}
                
			}
            
		}
        
	}
    
    if( mFft ) {
		freqData = mFft->getAmplitude();
		timeData = mFft->getData();
		dataSize = mFft->getBinSize();
    }
}

void Audio::draw()
{
	if ( mFft ) {
        
		float dataSizef = (float)dataSize;
        
		// Get dimensions
		float scale = ( (float)getWindowWidth() - 20.0f ) / dataSizef;
		float windowHeight = (float)getWindowHeight();
        
		// Use polylines to depict time and frequency domains
		PolyLine<Vec2f> freqLine;
		PolyLine<Vec2f> timeLine;
        
		// Iterate through data
		for ( int32_t i = 0; i < dataSize; i++ ) {
            
			// Do logarithmic plotting for frequency domain
			float logSize = math<float>::log( dataSizef );
			float x = (float)( ( math<float>::log( (float)i ) / logSize ) * dataSizef );
			float y = math<float>::clamp( freqData[ i ] * ( x / dataSizef ) * ( math<float>::log( ( dataSizef - (float)i ) ) ), 0.0f, 2.0f );
            
			freqLine.push_back( Vec2f(        x * scale + 10.0f,            -y * ( windowHeight - 20.0f ) * 0.25f + ( windowHeight - 10.0f ) ) );
			timeLine.push_back( Vec2f( (float)i * scale + 10.0f, timeData[ i ] * ( windowHeight - 20.0f ) * 0.25f + ( windowHeight * 0.5f ) ) );
            
		}
        
        gl::color( 1.0, 0.0, 0.8, 0.5);
		gl::draw( freqLine );
		gl::draw( timeLine );
	}    
}

void Audio::shutdown()
{
    mTrack->enablePcmBuffering( false );
	mTrack->stop();
	if ( mFft ) {
		mFft->stop();
	}    
}
