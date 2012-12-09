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
#include "FaderPack.h"

class FaderPack;
class Configuration;

class Fader {
public:
	void setup(FaderPack *faderPack);
	void update();
	void shutdown();

    void stop();
    void pause();
    void resume();

    bool isActive();
    
    // XXX: temporary faders
    // XXX: void setCurve(int curveType);
    // XXX: void bindFinishedCallback();
    void bindParam(double *param);
    
    void fade(double dest, double dur);

private:
    FaderPack *mMaster;
    
    bool active;
    
    double *param;
    double startVal;
    double destVal;

    double startTime;
    double durTime;
};
