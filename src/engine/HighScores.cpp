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
#include "../Base64.h"

#ifdef _WIN32
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#endif

#define WIN32_SCORE_FILE "lvs.ini"

using namespace std;

void HighScores::setup( Configuration *config )
{
    topScore = DUMMY_SCORE;

    mConfig = config;
    loadScoreFile();
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
    
    return encrypted;
}

inline void HighScores::loadScoreFile() {
    std::string scoreStr;
    
#ifdef __APPLE__
    scoreStr = loadScoreFromPrefs();
#endif
    
#ifdef _WIN32
    scoreStr = loadScoreWin32();
#endif
    
    topScore = decodeScore(scoreStr);
}

inline void HighScores::saveScoreFile() {
    string highScore = encodeScore(topScore);
    
#ifdef __APPLE__
    saveScoreToPrefs(highScore);
#endif
    
#ifdef _WIN32
    saveScoreWin32(highScore);
#endif
}

string HighScores::encodeScore(int score) {
    string scoreStr = "LONGCATISLONG" + toString(score);
    string xorStr = xorString(scoreStr);
    string encodedStr = base64_encode((unsigned char const*)xorStr.c_str(), xorStr.length());

    return encodedStr;
}

int HighScores::decodeScore(string score_str) {
    string decoded   = base64_decode(score_str);
    string decrypted = xorString(decoded);
    
    int newScore = DUMMY_SCORE;
    try
    {
        newScore = std::stoi(decrypted.substr(13,decrypted.length()));
    }
    catch (...)
    {
        // cout << "Unable to decode high score, using default.\n";
        newScore = DUMMY_SCORE;
    }

    return newScore;
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

#ifdef _WIN32
void HighScores::saveScoreWin32(string score) {
	using boost::property_tree::ptree;

	ptree pt;
	pt.put("lvs.score", score);

	write_ini( WIN32_SCORE_FILE, pt );
}

string HighScores::loadScoreWin32() {
	using boost::property_tree::ptree;

	ptree pt;
	boost::property_tree::ini_parser::read_ini(WIN32_SCORE_FILE, pt);
	return pt.get<std::string>("lvs.score");
}
#endif
