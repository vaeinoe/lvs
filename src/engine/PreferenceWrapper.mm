#include <Foundation/Foundation.h>
#include "PreferenceWrapper.h"

void saveScoreToPrefs(std::string scoreStr) {
    NSString* result = [NSString stringWithUTF8String:scoreStr.c_str()];
    NSUserDefaults *prefs = [NSUserDefaults standardUserDefaults];
    [prefs setObject:result forKey:@"topScore"];
}
