//
//  AudioEngine.cpp
//  Leavs
//
//  Created by vae on 06/08/2013.
//  Copyright (c) 2013 Lumeet. All rights reserved.
//

#include "AudioEngine.h"

#define NUMBERSECOND 10
#define SAMPLE_RATE 44100
#define NBCHANNEL 2
#define BLOCKSIZE 512
#define TICK 8
#define PATCH_FILE "lvs.pd"

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
    
    initAudio();

    analyzer = new AudioAnalyzer();
    analyzer->setup(config);
}

float* AudioEngine::getFreqData()  { return analyzer->freqData; }
int32_t AudioEngine::getDataSize() { return analyzer->dataSize; }

inline void AudioEngine::processFloat(int numTicks, float *inputBuffer, float *outputBuffer) {
    src->processFloat(numTicks, inputBuffer, outputBuffer);
}

inline void AudioEngine::updateAnalyzer(const void * data, int byteCount) {
    analyzer->updateData(data, byteCount, SAMPLE_RATE);
}

void AudioEngine::initPD() {
    src = new PdBase();
    src->init(0, NBCHANNEL, SAMPLE_RATE);
    src->openPatch(PATCH_FILE, getAssetPath("").string());
    initPDArrays();
    
    src->computeAudio(true);
}

void AudioEngine::initPDArrays() {
    // TODO: we can't do this conveniently like this, need to use resource macros...
    
    string files [29] = { "00_00", "00_01", "00_02",
                          "01_01", "01_02", "01_03", "01_04", "01_u1",
                          "02_01", "02_02", "02_03", "02_04", "02_u1",
                          "03_01", "03_02", "03_03", "03_04", "03_u1",
                          "04_01", "04_02", "04_03", "04_04", "04_05", "04_u1",
                          "99_01", "99_02", "99_03", "99_04", "99_99" };
    
    for (int i = 0; i < 29; i++) {
        string filename = files[i] + ".ogg";
        string arr_l    = files[i] + "_l";
        string arr_r    = files[i] + "_r";
        
        cout << "filename " << filename << ", arr_l " << arr_l << ", arr_r" << arr_r << "\n";
    }
}

void AudioEngine::initPA() {
    // Initialize PortAudio
	PaStreamParameters inputParameters, outputParameters;
    PaError err;
    
    err = Pa_Initialize();
    if( err != paNoError ) portAudioError(err);
    
	// input
    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) portAudioError(err);
    
    inputParameters.channelCount = NBCHANNEL;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    
	// output
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) portAudioError(err);
    
    outputParameters.channelCount = NBCHANNEL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    err = Pa_OpenStream(&stream,
                        &inputParameters,
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

void AudioEngine::initAudio() {
    initPD();
    initPA();

//    cout << "Audio engine initialized" << endl;
}

void AudioEngine::update() {
    analyzer->update();
    processMessages();
}
void AudioEngine::draw()   { analyzer->draw(2, 40); }

void AudioEngine::processMessages() {
}

void AudioEngine::shutdown() {
    Pa_StopStream(stream);
    Pa_Terminate();
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
void AudioEngine::e_gameLose()   { src->sendBang("to_mainmenu"); src->sendBang("reset_all"); } // TODO: is this OK? TEST!
void AudioEngine::e_timeEnding() { src->sendBang("time_ending"); } // TODO!
void AudioEngine::e_timeNormal() { src->sendBang("time_normal"); } // TODO!

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
    src->startMessage();
    src->addFloat((float)pos.x);
    src->addFloat((float)pos.y);
    src->finishList("tile_move");
}
