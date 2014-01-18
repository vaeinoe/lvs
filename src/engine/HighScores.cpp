//
//  HighScores.cpp
//  Leavs
//
//  Created by vae on 22/04/2013.
//  Copyright (c) 2013 Lumeet. All rights reserved.
//

#include "HighScores.h"
#include <iostream>
#include <fstream>
using namespace std;

void HighScores::setup( Configuration *config )
{
    mConfig = config;
    loadScoreFile();
    
    topScore = DUMMY_SCORE;
    
    encodeScore(topScore);
}

string HighScores::xorString(string inStr) {
    const char* str = inStr.c_str();
    
    size_t messagelen = strlen(str);
    size_t keylen = strlen(HIGHSCORE_KEY);
    
    char* encrypted = (char*)malloc(messagelen + 1);
    
    int i;
    for(i = 0; i < messagelen; i++) {
        encrypted[i] = str[i] ^ HIGHSCORE_KEY[i % keylen];
    }
    encrypted[messagelen] = '\0';
    
//    cout << "Key: " << str  << "\n";
//    cout << "Encrypted key: " << encrypted << "\n\n";
    
    return encrypted;
}

inline void HighScores::loadScoreFile() {
    string line;
    string scoreStr;
    ifstream scores (toString(cinder::app::getAssetPath(SCORE_FILE)));
    
    // TODO: have to read all lines, encryption can cause \n
    if (scores.is_open())
    {
        getline(scores,line);
        topScore = decodeScore(line);
        scores.close();
    }
    else cout << "Unable to open score file for reading." << "\n";
}

inline void HighScores::saveScoreFile() {
    ofstream scores (toString(cinder::app::getAssetPath(SCORE_FILE)));
    if (scores.is_open())
    {
        scores << encodeScore(topScore);
        scores.close();
    }
    else cout << "Unable to open score file for writing.";
}

string HighScores::encodeScore(int score) {
    string scoreStr = "LONG CAT IS: " + toString(score);
    return xorString(scoreStr);
}

int HighScores::decodeScore(string score_str) {
    string decrypted = xorString(score_str);
    
//    int newScore = std::stoi(decrypted.substr(12,decrypted.length()));
//    cout << "Decrypted score: " << toString(newScore);
    
//    return newScore;
    return 9999;
}

int HighScores::getScore() {
    return topScore;
}

void HighScores::shutdown()
{
    saveScoreFile();
}

void HighScores::saveScore(int score) {
    if (score < topScore) {
        topScore = score;
        saveScoreFile();
    }
}