#include <Foundation/Foundation.h>
#include "PreferenceWrapper.h"

void saveScoreToPrefs(std::string scoreStr) {
    NSString* result = [NSString stringWithUTF8String:scoreStr.c_str()];
    NSUserDefaults *prefs = [NSUserDefaults standardUserDefaults];
    [prefs setObject:result forKey:@"mysteryMeat"];
    [prefs synchronize];
}

std::string loadScoreFromPrefs() {
    NSUserDefaults *prefs = [NSUserDefaults standardUserDefaults];
    NSString* result = [prefs objectForKey:@"mysteryMeat"];
    if (!result) {
        return "";
    }
    else {
        std::string *res = new std::string([result UTF8String]);
        return *res;
    }
}
