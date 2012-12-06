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

#define NUMTRACKS 5

class AudioTrack;

class Audio {
public:
	void setup(Configuration *config);
	void update();
	void draw();
	void shutdown();

    void fadeToPreset(int presetId, double fadeSec);
    
    float* getFreqData();
    int32_t getDataSize();
    
    AudioTrack *mTracks[32];
private:
    Configuration *mConfig;
    
    const double presets[2][5] = {{ 1.0, 0.0, 0.0, 0.0, 0.0 }, { 0.0, 1.0, 1.0, 1.0, 1.0 }};
};
