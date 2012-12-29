//
//  Fader.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/9/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "Fader.h"
#include "FaderPack.h"

void Fader::setup(FaderPack *faderPack, int newType)
{
    paramSet = false;
    
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
        double diffTime = mMaster->getSeconds() - startTime;
        if (diffTime <= durTime) {
            durTime -= diffTime;
//            cout << durTime << endl;
        }
        else {
            durTime = 0;
        }
    }
}

void Fader::resume() {
    if (!active) {
        startTime = mMaster->getSeconds();
//        cout << startTime << " STARTED" << endl;
        active = true;
    }
}

void Fader::addObserver(FadeObserver *obs)
{
    mObs.push_back(obs);
}

void Fader::addTime(double seconds) { durTime += seconds; }

double Fader::timeElapsed() {
    return mMaster->getSeconds() - startTime;
}

// TODO: not the best way to do this...
double Fader::timeLeft() {
    double diff = durTime - (mMaster->getSeconds() - startTime);
    if (diff < 0) { return 0; }
    else          { return diff; }
}

double Fader::timeTotal() {
    return durTime;
}

void Fader::bindParam(double *newParam)
{
    param = newParam;
    paramSet = true;
}

void Fader::fade(double dest, double dur)
{
    if (mMaster->isActive()) {
        if (paramSet) {
            startVal = *param;
            destVal = dest;
        }
        
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
            if (paramSet) { *param = destVal; }
            active = false;
            for( vector<FadeObserver*>::iterator t = mObs.begin(); t != mObs.end(); ++t ){
                (*t)->onFadeEnd(typeId);
            }

        }
        else if (paramSet) {
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
