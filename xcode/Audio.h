/*
 *  Audio.h
 *  Leavs
 *
 *  Created by Väinö Ala-Härkönen on 10/28/12.
 *  Copyright 2012 Lumeet. All rights reserved.
 *
 */
#pragma once
#include "Common.h"

class Audio {
public:
	void setup(Configuration *config);
	void update();
	void draw();
	void shutdown();

	ci::audio::SourceRef		mAudioSource;
	ci::audio::PcmBuffer32fRef	mBuffer;
	ci::audio::TrackRef			mTrack;
    
	KissRef						mFft;    

    float *freqData;
    float *timeData;
    int32_t dataSize;
private:
    Configuration *mConfig;
};
