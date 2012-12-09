//
//  FaderPack.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/9/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "FaderPack.h"

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
            (*t)->update();
        }
    }
}

void FaderPack::shutdown()
{
    active = false;
    mTimer->stop();
    mFaders.clear();
}

Fader* FaderPack::createFader()
{
    Fader *fader = new Fader();
    fader->setup(this);
    mFaders.push_back(fader);
    
    return fader;
}

bool FaderPack::isActive() { return active; }
double FaderPack::getSeconds() { return time; }

