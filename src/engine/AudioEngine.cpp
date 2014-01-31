//
//  AudioEngine.cpp
//  Leavs
//
//  Created by vae on 06/08/2013.
//  Copyright (c) 2013 Lumeet. All rights reserved.
//

#include "AudioEngine.h"
#include "Resources.h"
#include "../stb_vorbis.h"
#include <vector>
#include <stdlib.h>

#define NUMBERSECOND 10
#define SAMPLE_RATE 44100
#define NBCHANNEL 2
#define BLOCKSIZE 512
#define TICK 8
#define PATCH_FILE "lvs.pd"
#define WIN32_PATH "content\\"

using namespace chunkware_simple;

int pa_callback(const void *inputBuffer, void *outputBuffer,
                unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
                PaStreamCallbackFlags statusFlags, void *userData ) {
    AudioEngine *engine = ((AudioEngine*)userData);
    engine->processFloat(TICK, NULL, (float *)outputBuffer);
    engine->updateAnalyzer(outputBuffer, framesPerBuffer);

    return 0;
}

void AudioEngine::setup (Configuration *config) {
    mConfig = config;
    loadState = 0;
    limiter = new SimpleLimit();
    limiter->setThresh(-0.1);
    limiter->setSampleRate(SAMPLE_RATE);
    limiter->setAttack(1.0);
    limiter->setRelease(10.0);
    limiter->initRuntime();
}

float* AudioEngine::getFreqData()  { return analyzer->freqData; }
int32_t AudioEngine::getDataSize() { return analyzer->dataSize; }

inline void AudioEngine::processFloat(int numTicks, float *inputBuffer, float *outputBuffer) {
    src->processFloat(numTicks, inputBuffer, outputBuffer);
    limit(outputBuffer);
}

inline void AudioEngine::limit(float *buf) {
    int i, j;
    int size = BLOCKSIZE;
    for (i = 0, j = 0; i < size; i++, j += 2)
    {
        double l = (double)buf[j];
        double r = (double)buf[j+1];
        limiter->process(l,r);

        if      ( l > 1  ) l = 1;
        else if ( l < -1 ) l = -1;
        
        if      ( r > 1  ) r = 1;
        else if ( r < -1 ) r = -1;
        
        buf[j] = (float)l;
        buf[j+1] = (float)r;
    }
}

inline void AudioEngine::updateAnalyzer(const void * data, int byteCount) {
    analyzer->updateData(data, byteCount, SAMPLE_RATE);
}

void AudioEngine::initAudio() {
    string files[30] = { "00_00", "00_01", "00_02",
                         "01_01", "01_02", "01_03", "01_04", "01_u1",
                         "02_01", "02_02", "02_03", "02_04", "02_u1",
                         "03_01", "03_02", "03_03", "03_04", "03_u1",
                         "04_01", "04_02", "04_03", "04_04", "04_05", "04_u1",
                         "99_01", "99_02", "99_03", "99_04", "99_99", "rd" };

    DataSourceRef ref;
	Patch patch;
	bool success;

    switch (loadState) {
        case 0:
            analyzer = new AudioAnalyzer();
            analyzer->setup(mConfig);

            src = new PdBase();
            success = src->init(0, NBCHANNEL, SAMPLE_RATE);
            assert(success == true);

#ifdef __APPLE__
            patch = src->openPatch(PATCH_FILE, App::getResourcePath().string());
            assert(patch.isValid() == true);
#endif
#ifdef _WIN32
            src->addToSearchPath(toString(getAppPath()));
            src->addToSearchPath(WIN32_PATH);

            patch = src->openPatch(PATCH_FILE, WIN32_PATH);
            assert(patch.isValid() == true);
#endif
            
            break;
        case 1:
            for (int i = 0; i < 8; i++) {
                ref = getAudioResource(files[i]);
                loadOGGToArray(ref, files[i]);
            }

            for (int i = 8; i < 18; i++) {
                ref = getAudioResource(files[i]);
                loadOGGToArray(ref, files[i]);
            }
            
            break;
        case 2:
            for (int i = 18; i < 30; i++) {
                ref = getAudioResource(files[i]);
                loadOGGToArray(ref, files[i]);
            }
            src->computeAudio(true);
            initPA();

            break;
    }

    loadState++;
}

void AudioEngine::loadOGGToArray(DataSourceRef ref, string id) {
    string arr_l    = id + "_l";
    string arr_r    = id + "_r";

    assert(ref);

    Buffer buf = ref->getBuffer();
    size_t dataSize = buf.getDataSize();
    unsigned char *data = (unsigned char *)(buf.getData());
        
    short *sample_buf;
    int channels = 0;
    
    int sample_count = stb_vorbis_decode_memory(data, dataSize, &channels, &sample_buf);
    
    assert(sample_count > 0);
    
    float** read_samples = (float**)calloc(2, sizeof(float*));
    read_samples[0] = (float*)calloc(sample_count, sizeof(float));
    read_samples[1] = (float*)calloc(sample_count, sizeof(float));
    
    assert(channels == 2);
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < sample_count; j++) {
            read_samples[i][j] = (float)sample_buf[2 * j + i] * (1.0f / 32768.0f);
        }
    }
    
    //cout << "buffer id: " << id << " data size: " << toString(dataSize) << ", channels: " << channels << ", samples per channel: "
    //<< toString(sample_count) << "\n";

    bool success_l, success_r;
    
    src->startMessage();
    src->addFloat(sample_count);
    src->finishMessage(arr_l, "resize");
    src->clearArray(arr_l);

    src->startMessage();
    src->addFloat(sample_count);
    src->finishMessage(arr_r, "resize");
    src->clearArray(arr_r);
    
    std::vector<float> s1(read_samples[0], read_samples[0] + sample_count - 1);
    success_l = src->writeArray(arr_l, s1);
    
    std::vector<float> s2(read_samples[1], read_samples[1] + sample_count - 1);
    success_r = src->writeArray(arr_r, s2);
    
    free(sample_buf);
    free(read_samples[0]);
    free(read_samples[1]);
    free(read_samples);
}

void AudioEngine::initPA() {
    // Initialize PortAudio

    PaStreamParameters inputParameters, outputParameters;
    PaError err;
    
    err = Pa_Initialize();
    if( err != paNoError ) portAudioError(err);

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */	
    if (inputParameters.device == paNoDevice) portAudioError(err);
    
    inputParameters.channelCount = NBCHANNEL;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) portAudioError(err);
    
    outputParameters.channelCount = NBCHANNEL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(&stream,
                        NULL,
                        &outputParameters,
                        SAMPLE_RATE,
                        BLOCKSIZE,
                        paClipOff,
                        pa_callback,
                        this);
    if( err != paNoError ) portAudioError(err);
    
    err = Pa_StartStream( stream );
    if( err != paNoError ) portAudioError(err);
}

void AudioEngine::update() {
    analyzer->update();
    processMessages();
}
void AudioEngine::draw()   { analyzer->draw(1, 40); }

void AudioEngine::processMessages() {
}

void AudioEngine::shutdown() {
    Pa_StopStream(stream);
    Pa_Terminate();
    //src->clear();
    //delete src;
}

void AudioEngine::portAudioError(PaError err) {
    cout << "Could not initialize audio engine: " << Pa_GetErrorText(err) << endl;
    Pa_Terminate();
}

// ================= EVENTS

void AudioEngine::e_gameLoaded() { src->sendBang("to_mainmenu"); }
void AudioEngine::e_gameStart()  { src->sendBang("reset_all"); src->sendBang("to_game"); }
void AudioEngine::e_gamePause()  { src->sendBang("to_mainmenu"); }
void AudioEngine::e_gameResume() { src->sendBang("to_game"); }
void AudioEngine::e_gameQuit()   { src->sendBang("quit_game"); }
void AudioEngine::e_gameWin()    { src->sendBang("win_game"); }
void AudioEngine::e_gameLose()   { src->sendBang("to_mainmenu"); src->sendBang("reset_all"); }
void AudioEngine::e_timeEnding() { src->sendBang("time_ending"); }
void AudioEngine::e_timeNormal() { src->sendBang("time_normal"); }

void AudioEngine::e_scoreChange(int change, int score) {
    src->startMessage();
    src->addFloat(change);
    src->addFloat(score);
    src->finishList("score_change");
}

void AudioEngine::e_levelUp(int tile, int level) {
    if      (tile == 0) { src->sendBang("level_up_1"); }
    else if (tile == 1) { src->sendBang("level_up_2"); }
    else if (tile == 2) { src->sendBang("level_up_3"); }
    else if (tile == 3) { src->sendBang("level_up_4"); }
}

void AudioEngine::e_tileDestroy(int tile) {
    if      (tile == 0) { src->sendBang("hit_1"); }
    else if (tile == 1) { src->sendBang("hit_2"); }
    else if (tile == 2) { src->sendBang("hit_3"); }
    else if (tile == 3) { src->sendBang("hit_4"); }
}

void AudioEngine::e_tileMove(Vec2i pos) {
//    src->startMessage();
//    src->addFloat((float)pos.x);
//    src->addFloat((float)pos.y);
//    src->finishList("tile_move");
}

DataSourceRef AudioEngine::getAudioResource(string identifier) {
    if (identifier == "00_00") return loadResource(RES_00_00);
    if (identifier == "00_01") return loadResource(RES_00_01);
    if (identifier == "00_02") return loadResource(RES_00_02);
    
    if (identifier == "01_01") return loadResource(RES_01_01);
    if (identifier == "01_02") return loadResource(RES_01_02);
    if (identifier == "01_03") return loadResource(RES_01_03);
    if (identifier == "01_04") return loadResource(RES_01_04);
    if (identifier == "01_u1") return loadResource(RES_01_u1);
    
    if (identifier == "02_01") return loadResource(RES_02_01);
    if (identifier == "02_02") return loadResource(RES_02_02);
    if (identifier == "02_03") return loadResource(RES_02_03);
    if (identifier == "02_04") return loadResource(RES_02_04);
    if (identifier == "02_u1") return loadResource(RES_02_u1);

    if (identifier == "03_01") return loadResource(RES_03_01);
    if (identifier == "03_02") return loadResource(RES_03_02);
    if (identifier == "03_03") return loadResource(RES_03_03);
    if (identifier == "03_04") return loadResource(RES_03_04);
    if (identifier == "03_u1") return loadResource(RES_03_u1);

    if (identifier == "04_01") return loadResource(RES_04_01);
    if (identifier == "04_02") return loadResource(RES_04_02);
    if (identifier == "04_03") return loadResource(RES_04_03);
    if (identifier == "04_04") return loadResource(RES_04_04);
    if (identifier == "04_05") return loadResource(RES_04_05);
    if (identifier == "04_u1") return loadResource(RES_04_u1);

    if (identifier == "99_01") return loadResource(RES_99_01);
    if (identifier == "99_02") return loadResource(RES_99_02);
    if (identifier == "99_03") return loadResource(RES_99_03);
    if (identifier == "99_04") return loadResource(RES_99_04);
    if (identifier == "99_99") return loadResource(RES_99_99);
    
    if (identifier == "rd") return loadResource(RES_RD);

    cout << "No resource found for id " << identifier << "\n";
    return NULL;
}
