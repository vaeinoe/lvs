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
#ifdef CINDER_MSW
    audioEngine = createIrrKlangDevice(ESOD_WIN_MM);
#else
    audioEngine = createIrrKlangDevice();
#endif
    audioEngine->setSoundVolume(MAIN_VOLUME);
    
    analyzer = new AudioAnalyzer();
	analyzer->setup();

    audioEngine->setMixedDataOutputReceiver(analyzer);

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
    analyzer->update();
}

float* Audio::getFreqData() { return analyzer->freqData; }
int32_t Audio::getDataSize() { return analyzer->dataSize; }

void Audio::draw() { analyzer->draw(1, -300); }

void Audio::shutdown()
{
    for (int i = 0; i < NUMTRACKS; i++) { mTracks[i]->shutdown(); }
    audioEngine->drop();
}
