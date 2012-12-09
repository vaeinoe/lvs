//
//  Fader.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/9/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "Fader.h"

void Fader::setup(FaderPack *faderPack, int newType)
{
    mMaster = faderPack;
    active = false;
    
    startVal = 0.0;
    destVal = 0.0;
    
    startTime = 0.0;
    durTime = 0.0;
    
    typeId = newType;
}

void Fader::pause() {
    if (active) {
        active = false;
        double diffTime = startTime - mMaster->getSeconds();
        if (diffTime <= durTime) {
            durTime -= diffTime;
        }
    }
}

void Fader::resume() {
    if (!active) {
        startTime = mMaster->getSeconds();
        active = true;
    }
}

void Fader::addObserver(FadeObserver *obs)
{
    mObs.push_back(obs);
}

void Fader::bindParam(double *newParam) { param = newParam; }

void Fader::fade(double dest, double dur)
{
    if (mMaster->isActive() && param != NULL) {
        startVal = *param;
        destVal = dest;
        
        startTime = mMaster->getSeconds();
        durTime = dur;
        
        active = true;
    }
}

void Fader::update()
{
    if (active && mMaster->isActive()) {
        double diffTime = mMaster->getSeconds() - startTime;
        if (diffTime > durTime) {
            *param = destVal;
            active = false;
            for( vector<FadeObserver*>::iterator t = mObs.begin(); t != mObs.end(); ++t ){
                (*t)->onFadeEnd(typeId);
            }

        }
        else {
            double fadePoint = diffTime / durTime;
            if (startVal < destVal) {
                *param = startVal + (fadePoint * (destVal - startVal));
            }
            else if (startVal > destVal) {
                *param = startVal - (fadePoint * (startVal - destVal));
            }
        }
    }
}

bool Fader::isActive() {
    if (active) return active;
    return false;
}

void Fader::stop() { active = false; }

void Fader::shutdown() { } // ...
