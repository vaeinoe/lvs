//
//  AudioTrack.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/4/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#define TYPE_PRC
#define TYPE_PAD
#define TYPE_SEQ
#define TYPE_DRS

#include "AudioTrack.h"

void AudioTrack::setup(Audio *audio, int trackNo, bool looping)
{
    sprintf(name, "track%d", trackNo);
    
    mAudio = audio;
    
    fadeTimer   = new Timer();
    fading      = false;
    fadeSrcVol  = 0.0;
    fadeDestVol = 0.0;
    fadeTimeSec = 0;
    
    for (int i = 0; i < 5; i++) {
        PolyLine<Vec2f> line;
        prevLines[i] = line;
    }
    
    DataSourceRef trackRef = loadTrack( trackNo );
    Buffer trackBuf = trackRef->getBuffer();
    mSoundSource = audio->audioEngine->addSoundSourceFromMemory(trackBuf.getData(), trackBuf.getDataSize(), name);
    
    mTrack = audio->audioEngine->play2D(name, true, true, true);
    //mTrack       = audio::Output::addTrack( mAudioSource, false );
    //mTrack->enablePcmBuffering(true);
    //mTrack->setLooping(looping);
    
    fftRunning = false;
}

void AudioTrack::fadeTo(float destVol, double fadeSec)
{
    fading = true;
    fadeSrcVol = mTrack->getVolume();
    fadeDestVol = destVol;
    fadeTimeSec = fadeSec;
    
    fadeTimer->start();
}

void AudioTrack::update()
{
    if (fading && !fadeTimer->isStopped()) {
        double time = fadeTimer->getSeconds();
        if (time > fadeTimeSec) {
            mTrack->setVolume(fadeDestVol);
            fading = false;
            fadeTimer->stop();
        }
        else {
            double fadePoint = time / fadeTimeSec;
            if (fadeSrcVol < fadeDestVol) {
                mTrack->setVolume(fadeSrcVol + (fadePoint * (fadeDestVol - fadeSrcVol)));
            }
            else if (fadeSrcVol > fadeDestVol) {
                mTrack->setVolume(fadeSrcVol - (fadePoint * (fadeSrcVol - fadeDestVol)));
            }
        }
    }
/*
    if ( !mTrack->getIsPaused()) {
        mBuffer = mTrack->getPcmBuffer();
        if ( mBuffer && mBuffer->getInterleavedData() ) {
            
            uint32_t sampleCount = mBuffer->getInterleavedData()->mSampleCount;
            if ( sampleCount > 0 ) {
                if (fftRunning)
                    if ( !mFft ) {
                        mFft = Kiss::create( sampleCount );
                    }
                
                if ( mBuffer->getInterleavedData()->mData != 0 ) {
                    mFft->setData( mBuffer->getInterleavedData()->mData );
                }
            }
        }
    }

    if( fftRunning && mFft ) {
        freqData = mFft->getAmplitude();
        timeData = mFft->getData();
        dataSize = mFft->getBinSize();
    }
    */
}

void AudioTrack::draw(float scaleIn, float offsetIn)
{
	if ( mFft ) {
        
		float dataSizef = (float)dataSize;
        
		// Get dimensions
		float scale = (( (float)getWindowWidth() - 20.0f ) / dataSizef) * scaleIn;
		float windowHeight = (float)getWindowHeight() + offsetIn;
        
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
			timeLine.push_back( Vec2f( (float)i * scale + 10.0f,  mTrack->getVolume() * timeData[ i ] * ( windowHeight - 20.0f ) * 0.25f + ( windowHeight * 0.5f ) ) );
            
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


void AudioTrack::play(double fadeSec)
{
    mTrack->setIsPaused(false);
    mTrack->setVolume(0);
    fadeTo(1.0, fadeSec);
}

void AudioTrack::stop(double fadeSec)
{
    fadeTo(0.0, fadeSec);
    mTrack->stop();
}

bool AudioTrack::isPlaying()    { return mTrack->isFinished(); }
void AudioTrack::startFft()     { fftRunning = false; }
void AudioTrack::stopFft()      { fftRunning = false; }
bool AudioTrack::isFftRunning() { return fftRunning; }

void AudioTrack::shutdown()
{
    mTrack->stop();

	if ( mFft ) {
		mFft->stop();
	}
}

// Ugly hack due to the way Cinder handles resources
DataSourceRef AudioTrack::loadTrack(int trackNo) {
    /*
     2 random track sets, each:
       - 2 sequences (1,2)
       - 1 pad (3)
       - 1 melody (4)
       - 1 bass (5)
       - 1 drums (6)
       - 2 percussion (7,8)
    */
    switch(trackNo) {
        case 0:
            return loadResource( RES_SEQ001 );
        case 1:
            return loadResource( RES_SEQ002 );
        case 2:
            return loadResource( RES_PRC003 );
        case 3:
            return loadResource( RES_PRC002 );
        case 4:
            return loadResource( RES_PRC001 );
        case 5:
            return loadResource( RES_SEQ003 );
        default:
            return loadResource( RES_PAD001 );
    }
}