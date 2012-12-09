//
//  Fader.cpp
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/9/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#include "Fader.h"

void Fader::setup(FaderPack *faderPack)
{
    mMaster = faderPack;
    active = false;
    
    startVal = 0.0;
    destVal = 0.0;
    
    startTime = 0.0;
    durTime = 0.0;
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

void Fader::bindParam(double *newParam) { param = newParam; }

void Fader::fade(double dest, double dur)
{
    if (mMaster->isActive() && param) {
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
        double diffTime = startTime - mMaster->getSeconds();
        if (diffTime > durTime) {
            *param = destVal;
            active = false;
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

bool Fader::isActive() { return active; }
void Fader::stop() { active = false; }

void Fader::shutdown() { } // ...
