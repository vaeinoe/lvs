//
//  AudioFx.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/10/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Common.h"
#include "Configuration.h"
#include "irrKlang.h"
#include "FaderPack.h"
#include "Fader.h"
#include "FadeObserver.h"
#include "Audio.h"
using namespace irrklang;

class Audio;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

class AudioFx {
public:
	void setup(Audio *master);
    void playTileFx(int tileType, int count);
    
private:
    vector<ISoundSource*> tileFx;
    void loadAll();
    
    Audio *mAudio;    
};


