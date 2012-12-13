//
//  AudioFx.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/10/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"
#include "irrKlang.h"

using namespace irrklang;

class Audio;

class AudioFx {
public:
	void setup(Audio *master);
    void playTileFx(int tileType, int count);
    
private:
    vector<ISoundSource*> tileFx;
    void loadAll();
    
    Audio *mAudio;    
};


