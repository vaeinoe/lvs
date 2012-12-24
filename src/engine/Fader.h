//
//  FaderPack.h
//  Leavs
//
//  Created by Väinö Ala-Härkönen on 12/9/12.
//  Copyright (c) 2012 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"
#include "FadeObserver.h"

class FaderPack;
class Configuration;

class Fader {
public:
	void setup(FaderPack *faderPack, int newType = 0);
	void update();
	void shutdown();

    void stop();
    void pause();
    void resume();

    bool isActive();
    
    void addTime(double seconds);
    double timeLeft();
    double timeTotal();
    
    // XXX: temporary faders
    // XXX: void setCurve(int curveType);
    // XXX: void bindFinishedCallback();
    void bindParam(double *param);
    void addObserver(FadeObserver *obs);
    
    void fade(double dest, double dur);

private:
    vector<FadeObserver*> mObs;
    FaderPack *mMaster;
    int typeId;
    
    bool active;
    
    double *param;
    bool paramSet;
    
    double startVal;
    double destVal;

    double startTime;
    double durTime;
};
