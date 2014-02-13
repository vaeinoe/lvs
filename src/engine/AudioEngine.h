//
//  AudioEngine.h
//  Leavs
//
//  Created by vae on 06/08/2013.
//  Copyright (c) 2013 Lumeet. All rights reserved.
//

#pragma once
#include "Configuration.h"
#include "PdBase.hpp"
#include "cinder/audio/Output.h"
#include "cinder/audio/Callback.h"
#include "AudioAnalyzer.h"
#include "../chunkware/SimpleLimit.h"

using namespace pd;

class AudioEngine {
public:
	void setup(Configuration *config);
    void update();
    void draw();
	void shutdown();

    void initAudio();

    float* getFreqData();
    int32_t getDataSize();
    
    inline void processFloat(int numTicks, float *inputBuffer, float *outputBuffer);
    inline void updateAnalyzer(const void * data, int byteCount);

    // events. tacky.
    void e_gameLoaded();
    void e_gameStart();
    void e_gamePause();
    void e_gameResume();
    void e_gameQuit();

    void e_gameWin();
    void e_gameLose();
    
    void e_timeEnding(); // Game time running out
    void e_timeNormal(); // Game time back to normal

    void e_scoreChange(int change, int score);
    void e_levelUp(int tile, int level);
    
    void e_tileDestroy(int tile);
    void e_tileMove(Vec2i pos);

private:
    int loadState;
    
    void audioCallback( uint64_t inSampleOffset, uint32_t ioSampleCount, ci::audio::Buffer32f * ioBuffer );
    chunkware_simple::SimpleLimit *limiter;
    PdBase *src;
    
    void initPD();
    void initPDArrays();
    DataSourceRef getAudioResource(string identifier);
    void loadOGGToArray(DataSourceRef ref, string id);
    void initIO();
    void processMessages();
    void limit(float *buf);
    
    Configuration *mConfig;
    AudioAnalyzer *analyzer;
};
