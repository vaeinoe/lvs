//
//  AudioAnalyzer.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/8/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"
#include "KissFFT.h"

class AudioAnalyzer {
public:
    void setup(Configuration *config);
    void update();
    void draw(float scaleIn, float offsetIn);
    void updateData ( const void * data, int byteCount, int playbackrate );

    friend class AudioEngine;
private:
    Configuration *mConfig;
    KissRef	mFft;

    PolyLine<Vec2f> prevLines[5];

    int32_t audioSize;
    float *audioFloat;
    
    float *freqData;
    float *timeData;
    
    int32_t dataSize;
    int32_t sampleRate;
};