//
//  FaderPack.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/9/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "FaderPack.h"
#include "Fader.h"

void FaderPack::setup(Configuration *config, bool startNow)
{
    mConfig = config;
    mTimer = new Timer();

    if (startNow) start();
}

void FaderPack::start()
{
    mTimer->start();
    time = mTimer->getSeconds();
    active = true;
}

void FaderPack::stop()
{
    mTimer->stop();
    active = false;
}

void FaderPack::pause()
{
    if (active) {
        mTimer->stop();
        active = false;
        for( vector<Fader*>::iterator t = mFaders.begin(); t != mFaders.end(); ++t ){
            (*t)->pause();
        }
    }
}

void FaderPack::resume()
{
    if (!active) {
        mTimer->start();
        time = mTimer->getSeconds();
        active = true;
        for( vector<Fader*>::iterator t = mFaders.begin(); t != mFaders.end(); ++t ){
            (*t)->resume();
        }
    }
}

void FaderPack::update()
{
    if (active) {
        time = mTimer->getSeconds();
        for( vector<Fader*>::iterator t = mFaders.begin(); t != mFaders.end(); ++t ){
            if ((*t)->isActive()) { (*t)->update(); }
        }
    }
}

void FaderPack::shutdown()
{
    active = false;
    mTimer->stop();
    mFaders.clear();
}

Fader* FaderPack::createFader(int type)
{
    Fader *fader = new Fader();
    fader->setup(this, type);
    mFaders.push_back(fader);
    
    return fader;
}

Fader* FaderPack::createFader(int type, double *bindPar, FadeObserver *obs)
{
    Fader *fader = new Fader();
    
    fader->setup(this, type);
    fader->bindParam(bindPar);
    fader->addObserver(obs);
    
    mFaders.push_back(fader);
    
    return fader;
}


bool FaderPack::isActive() { return active; }
double FaderPack::getSeconds() { return time; }

