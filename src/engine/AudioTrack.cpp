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
#include "Audio.h"
#include "AudioAnalyzer.h"
#include "FaderPack.h"
#include "Fader.h"

void AudioTrack::setup(Audio *audio, int trackNo, bool looping)
{
    sprintf(name, "track%d", trackNo);
    
    mAudio = audio;
    
    volFader = mAudio->mConfig->faders->createFader(0, &volume, this);
    
    DataSourceRef trackRef = loadTrack( trackNo );
    Buffer trackBuf = trackRef->getBuffer();
    mSoundSource = audio->audioEngine->addSoundSourceFromMemory(trackBuf.getData(), trackBuf.getDataSize(), name);
    
    mTrack = audio->audioEngine->play2D(name, true, true, true);
    
    fftRunning = false;
}

void AudioTrack::fadeTo(float destVol, double fadeSec)
{
    fading = true;
    volFader->fade(destVol, fadeSec);
}

void AudioTrack::update()
{
    if (fading) { mTrack->setVolume(volume); }
}

void AudioTrack::draw(float scaleIn, float offsetIn)
{
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

void AudioTrack::shutdown() { mTrack->stop(); }

void AudioTrack::onFadeEnd(int typeId)
{
    mTrack->setVolume(volume);
    fading = false;
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
            return loadResource( RES_DRS001 );
        case 4:
            return loadResource( RES_PRC001 );
        case 5:
            return loadResource( RES_SEQ003 );
        default:
            return loadResource( RES_PAD001 );
    }
}