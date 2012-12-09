//
//  AudioAnalyzer.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/8/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Common.h"
#include "Configuration.h"
#include "AudioTrack.h"
#include "irrKlang.h"
using namespace irrklang;

class AudioAnalyzer: public ISoundMixedOutputReceiver {
public:
    void setup();
    void update();
    void draw(float scaleIn, float offsetIn);
    void OnAudioDataReady ( const void * data, int byteCount, int playbackrate );

    friend class Audio;
private:
    KissRef	mFft;

    PolyLine<Vec2f> prevLines[5];

    int16_t *audioData;
    int32_t audioSize;
    float *audioFloat;
    
    float *freqData;
    float *timeData;
    
    int32_t dataSize;
    int32_t sampleRate;
};