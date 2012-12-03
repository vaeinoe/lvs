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

    for (int i = 0; i < 5; i++) {
        PolyLine<Vec2f> line;
        prevLines[i] = line;
    }
    
    mAudioSource = audio::load( loadResource( "plink1.wav" ) );
    mAudioSource2 = audio::load( loadResource( "plink2.wav" ) );
    mAudioSource3 = audio::load( loadResource( "plink3.wav" ) );
    mAudioSource4 = audio::load( loadResource( "plink4.wav" ) );
    mAudioSource5 = audio::load( loadResource( "plink5.wav" ) );

	mTrack = audio::Output::addTrack( mAudioSource, false );
	mTrack2 = audio::Output::addTrack( mAudioSource2, false );
	mTrack3 = audio::Output::addTrack( mAudioSource3, false );
	mTrack4 = audio::Output::addTrack( mAudioSource4, false );
	mTrack5 = audio::Output::addTrack( mAudioSource5, false );

	mTrack->enablePcmBuffering( true );
    mTrack->setLooping( true );
    mTrack->play();

	mTrack2->enablePcmBuffering( true );
    mTrack2->setLooping( true );
	mTrack2->play();

	mTrack3->enablePcmBuffering( true );
    mTrack3->setLooping( true );
	mTrack3->play();

	mTrack4->enablePcmBuffering( true );
    mTrack4->setLooping( true );
	mTrack4->play();
    
	mTrack5->enablePcmBuffering( true );
    mTrack5->setLooping( true );
	mTrack5->play();

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
        
        for (int i = 0; i < 5; i++) {
            gl::color( 0.8, 0.0, 0.3, 0.5 * (i * 1.0 / 6) );
            gl::draw(prevLines[i]);
        }

        for (int i = 1; i < 5; i++) {
            prevLines[i-1] = prevLines[i];
        }
        prevLines[4] = timeLine;

        gl::color( 1.0, 0.4, 0.8, 0.6);
		//gl::draw( freqLine );
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
