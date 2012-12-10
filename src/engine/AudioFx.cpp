//
//  AudioFx.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/10/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "AudioFx.h"

void AudioFx::setup(Audio *master)
{
    mAudio = master;
    loadAll();
}

void AudioFx::playTileFx(int tileType, int count)
{
    mAudio->audioEngine->play2D("test");
}

void AudioFx::loadAll()
{
    DataSourceRef trackRef = loadResource( RES_TFX001 );
    Buffer trackBuf = trackRef->getBuffer();
    mAudio->audioEngine->addSoundSourceFromMemory(trackBuf.getData(), trackBuf.getDataSize(), "test");
}

