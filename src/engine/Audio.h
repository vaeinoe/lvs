/*
 *  Audio.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/28/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */
#pragma once
#include "Configuration.h"
#include "FadeObserver.h"
#include "Resources.h"
#include "irrKlang.h"

using namespace irrklang;

class AudioTrack;
class AudioAnalyzer;
class AudioFx;
class Configuration;
class Fader;

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
    
    AudioAnalyzer* analyzer;
    AudioFx* audioFx;
    ISoundEngine* audioEngine;
    
    Fader *mainVolFader;
    double mainVol;
    bool fading;
};


