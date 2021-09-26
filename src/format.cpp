#include <string>
#include <time.h>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    time_t time_seconds(seconds);
    tm *p = gmtime(&time_seconds);
    string result = std::to_string(p->tm_hour) + ":" + std::to_string(p->tm_min) + ":" + std::to_string(p->tm_sec);
    return result;
}