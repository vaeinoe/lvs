/*
 *  Audio.cpp
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/28/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */

#include "Audio.h"

void Audio::setup(Configuration *config)
{
    mConfig = config;

    // NUMTRACKS
    string audioFiles[] = {"plink1.wav", "plink2.wav", "plink3.wav",
                           "plink4.wav", "plink5.wav" };

    for (int i = 0; i < NUMTRACKS; i++) {
        mTracks[i] = new AudioTrack();
        mTracks[i]->setup(audioFiles[i], true);
    }
    
    for (int i = 0; i < NUMTRACKS; i++) {
        mTracks[i]->startFft();
        mTracks[i]->play(0);
    }
    
    fadeToPreset(0, 5.0);
}

void Audio::fadeToPreset(int presetId, double fadeSec) {
    if (presets[presetId]) {
        for (int i = 0; i < NUMTRACKS; i++) {
            mTracks[i]->fadeTo(presets[presetId][i], fadeSec);
        }
    }
}

void Audio::update()
{
    for (int i = 0; i < NUMTRACKS; i++) {
        mTracks[i]->update();
    }
}

float* Audio::getFreqData()
{
    return mTracks[0]->freqData;
}

int32_t Audio::getDataSize()
{
    return mTracks[0]->dataSize;
}


void Audio::draw()
{
    for (int i = 0; i < NUMTRACKS; i++) {
        mTracks[i]->draw(1, -200 * i);
    }
    
    //mTracks[0]->draw(2, -200);
    //mTracks[3]->draw(1, -400);
}

void Audio::shutdown()
{
    for (int i = 0; i < NUMTRACKS; i++) {
        mTracks[i]->shutdown();
    }
}
