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
#include "Configuration.h"

class Audio {
public:
	void setup(Configuration *config);
	void update();
	void draw();
	void shutdown();

	ci::audio::SourceRef		mAudioSource;
	ci::audio::SourceRef		mAudioSource2;
	ci::audio::SourceRef		mAudioSource3;
	ci::audio::SourceRef		mAudioSource4;
	ci::audio::SourceRef		mAudioSource5;

    
	ci::audio::TrackRef			mTrack;
	ci::audio::TrackRef			mTrack2;
	ci::audio::TrackRef			mTrack3;
	ci::audio::TrackRef			mTrack4;
	ci::audio::TrackRef			mTrack5;
    
	ci::audio::PcmBuffer32fRef	mBuffer;
	KissRef						mFft;

    float *freqData;
    float *timeData;
    int32_t dataSize;
private:
    Configuration *mConfig;
    
    PolyLine<Vec2f> prevLines[5];
};
