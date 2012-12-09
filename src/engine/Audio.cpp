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
    audioEngine->setSoundVolume(0.0);

    fadeTimer   = new Timer();
    fading      = false;
    fadeSrcVol  = 0.0;
    fadeDestVol = 0.0;
    fadeTimeSec = 0.0;
    
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
    
    fadeMainTo(MAIN_VOLUME, 0.5);
    fadeToPreset(1, 5.0);
}

void Audio::fadeMainTo(float destVol, double fadeSec)
{
    fading = true;
    fadeSrcVol = audioEngine->getSoundVolume();
    fadeDestVol = destVol;
    fadeTimeSec = fadeSec;
    
    fadeTimer->start();
}

void Audio::fadeToPreset(int presetId, double fadeSec) {
    if (presets[presetId]) {
        for (int i = 0; i < NUMTRACKS; i++) { mTracks[i]->fadeTo(presets[presetId][i], fadeSec); }
    }
}

void Audio::update()
{
    if (fading && !fadeTimer->isStopped()) {
        double time = fadeTimer->getSeconds();
        if (time > fadeTimeSec) {
            audioEngine->setSoundVolume(fadeDestVol);
            fading = false;
            fadeTimer->stop();
        }
        else {
            double fadePoint = time / fadeTimeSec;
            if (fadeSrcVol < fadeDestVol) {
                audioEngine->setSoundVolume(fadeSrcVol + (fadePoint * (fadeDestVol - fadeSrcVol)));
            }
            else if (fadeSrcVol > fadeDestVol) {
                audioEngine->setSoundVolume(fadeSrcVol - (fadePoint * (fadeSrcVol - fadeDestVol)));
            }
        }
    }

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
