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
	void setup(int trackNo, bool looping);
    
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
    DataSourceRef loadTrack(int trackNo);
    
    bool fftRunning;
    PolyLine<Vec2f> prevLines[5];
    
    Timer *fadeTimer;
    bool   fading;
    float  fadeSrcVol;
    float  fadeDestVol;
    double fadeTimeSec;
};
