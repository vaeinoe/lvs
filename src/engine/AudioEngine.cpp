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
#define BLOCKSIZE 256
#define TICK 4
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
    src->computeAudio(true);

    src->subscribe("test_foo");
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

    cout << "Audio engine initialized" << endl;
}

void AudioEngine::update() {
    analyzer->update();
    processMessages();
}
void AudioEngine::draw()   { analyzer->draw(4, 0); }

void AudioEngine::processMessages() {
    while(src->numMessages() > 0) {
        pd::Message& msg = src->nextMessage();
        switch(msg.type) {
            case PRINT:
                cout << "got print: " << msg.symbol << endl;
                break;
            case BANG:
                cout << "go a bang to " << msg.dest << endl;
                break;
            case LIST:
                cout << "got a list: " << msg.list << endl;
                break;
            case FLOAT:
                cout << "got a float " << msg.num << endl;
                break;
            case SYMBOL:
                cout << "got a symbol " << msg.symbol << endl;
                break;
            default:
                cout << "got an unspecified message!" << endl;
                break;
        }
    }
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
void AudioEngine::e_gameQuit()   { src->sendBang("game_quit"); }
void AudioEngine::e_gameWin()    { src->sendBang("game_win"); }
void AudioEngine::e_gameLose()   { src->sendBang("game_lose"); }
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
/*  src->startMessage();
    src->addFloat(tile);
    src->addFloat(level);
    src->finishList("level_up"); */
}

void AudioEngine::e_tileDestroy(Vec2i pos) {
    src->sendBang("hit");
//    src->startMessage();
//    src->addFloat((float)pos.x);
//    src->addFloat((float)pos.y);
//    src->finishList("tile_destroy");
}

void AudioEngine::e_tileMove(Vec2i pos) {
    src->startMessage();
    src->addFloat((float)pos.x);
    src->addFloat((float)pos.y);
    src->finishList("tile_move");
}
