/*
 *  Audio.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/28/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#include "Audio.h"

double Audio::presets[][NUMTRACKS] = {
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 1.0, 0.0, 0.0, 0.0},
    {1.0, 0.0, 1.0, 1.0, 1.0, 1.0}
};

void Audio::setup(Configuration *config)
{
    audioEngine = createIrrKlangDevice();

    mConfig = config;

    for (int i = 0; i < NUMTRACKS; i++) {
        mTracks[i] = new AudioTrack();
        mTracks[i]->setup(this, i, true);
    }
    
    for (int i = 0; i < NUMTRACKS; i++) {
        mTracks[i]->startFft();
        mTracks[i]->play(0);
    }
    
    fadeToPreset(1, 5.0);
}

void Audio::fadeToPreset(int presetId, double fadeSec) {
    if (presets[presetId]) {
        for (int i = 0; i < NUMTRACKS; i++) { mTracks[i]->fadeTo(presets[presetId][i], fadeSec); }
    }
}

void Audio::update()
{
    for (int i = 0; i < NUMTRACKS; i++) { mTracks[i]->update(); }
}

// TODO: swap analysis channel on demand
float* Audio::getFreqData() { return mTracks[0]->freqData; }
int32_t Audio::getDataSize() { return mTracks[0]->dataSize; }

void Audio::draw()
{
    for (int i = 0; i < NUMTRACKS; i++) {
        mTracks[i]->draw(1, -100 * i - 100);
    }
}

void Audio::shutdown()
{
    for (int i = 0; i < NUMTRACKS; i++) { mTracks[i]->shutdown(); }
    engine->drop();
}
