//
//  FaderPack.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/9/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"
#include "Common.h"
#include "Fader.h"
#include "FadeObserver.h"

class Fader;
class Configuration;

class FaderPack {
public:
	void setup(Configuration *config, bool start);

    void start();
    void stop();
    void pause();
    void resume();
    
	void update(); // Updates all faders
	void shutdown();
    
    bool isActive();
    double getSeconds();
    
    Fader* createFader(int type = 0);
    Fader* createFader(int type, double *bindPar, FadeObserver *obs);
    
private:
    bool active;
    
    Timer *mTimer;
    double time;
    
    Configuration *mConfig;
    
    vector<Fader*> mFaders;
};
