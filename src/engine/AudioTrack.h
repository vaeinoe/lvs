//
//  AudioTrack.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/4/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Common.h"
#include "Configuration.h"

class AudioTrack {
public:
	void setup(const string audioFile, bool looping);
    
	void update();
	void draw(float scale, float offset);
	void shutdown();
    
    void play(double fadeSec);
    void stop(double fadeSec);
    bool isPlaying();
    
    void fadeTo(float destVol, double fadeSec);
    
    void startFft();
    void stopFft();
    bool isFftRunning();
    
    ci::audio::SourceRef mAudioSource;
	ci::audio::TrackRef	 mTrack;
    
	ci::audio::PcmBuffer32fRef	mBuffer;
	KissRef						mFft;
    
    float *freqData;
    float *timeData;
    int32_t dataSize;
private:
    bool fftRunning;
    PolyLine<Vec2f> prevLines[5];
    
    Timer *fadeTimer   = new Timer();
    bool   fading        = false;
    float  fadeSrcVol   = 0.0;
    float  fadeDestVol  = 0.0;
    double fadeTimeSec   = 0;
};
