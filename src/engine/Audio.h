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
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

class AudioTrack;
class AudioAnalyzer;

class Audio {
public:
	void setup(Configuration *config);
	void update();
	void draw();
	void shutdown();

    void fadeToPreset(int presetId, double fadeSec);
    void fadeMainTo(float destVol, double fadeSec);
    
    float* getFreqData();
    int32_t getDataSize();
    
    AudioTrack *mTracks[32];
    
    friend class AudioTrack;
    
private:
    Configuration *mConfig;
	static double presets[][NUMTRACKS];
    
    ISoundEngine* audioEngine;
    AudioAnalyzer* analyzer;
    
    Fader *mainVolFader;
    double mainVol;
};


