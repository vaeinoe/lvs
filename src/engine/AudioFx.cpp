//
//  AudioFx.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/10/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "AudioFx.h"
#include "FaderPack.h"
#include "Fader.h"
#include "FadeObserver.h"
#include "Audio.h"

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

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

