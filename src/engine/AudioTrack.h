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
#include "AudioAnalyzer.h"
#include "irrKlang.h"
#include "FaderPack.h"
#include "Fader.h"
using namespace irrklang;

class Audio;

class AudioTrack : public FadeObserver {
public:
	void setup(Audio *audio, int trackNo, bool looping);
    
	void update();
	void draw(float scale, float offset);
	void shutdown();
    
    void play(double fadeSec);
    void stop(double fadeSec);
    bool isPlaying();
    
    void fadeTo(float destVol, double fadeSec);
    void onFadeEnd(int typeId);
    
    void startFft();
    void stopFft();
    bool isFftRunning();
    
    float *freqData;
    float *timeData;
    int32_t dataSize;
private:
    char name[100];
    
    Audio *mAudio;
    ISoundSource *mSoundSource;
    ISound *mTrack;
    
    bool mFft;
    
    DataSourceRef loadTrack(int trackNo);
    
    bool fftRunning;
    
    Fader *volFader;
    double volume;
    bool fading;
};
