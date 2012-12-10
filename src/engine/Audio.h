/*
 *  Audio.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/28/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */
#pragma once
#include "Common.h"
#include "Configuration.h"
#include "AudioTrack.h"
#include "AudioAnalyzer.h"
#include "irrKlang.h"
#include "FaderPack.h"
#include "Fader.h"
#include "FadeObserver.h"
#include "AudioFx.h"
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

class AudioTrack;
class AudioAnalyzer;
class AudioFx;

class Audio : public FadeObserver {
public:
	void setup(Configuration *config);
	void update();
	void draw();
	void shutdown();

    void fadeToPreset(int presetId, double fadeSec);
    void fadeMainTo(float destVol, double fadeSec);
    void playTileFx(int tileType, int count);
    
    float* getFreqData();
    int32_t getDataSize();
    
    AudioTrack *mTracks[32];

    void onFadeEnd(int typeId);
    
    friend class AudioTrack;
    friend class AudioFx;
    
private:
    Configuration *mConfig;
	static double presets[][NUMTRACKS];
    
    ISoundEngine* audioEngine;
    AudioAnalyzer* analyzer;
    AudioFx* audioFx;
    
    Fader *mainVolFader;
    double mainVol;
    bool fading;
};


