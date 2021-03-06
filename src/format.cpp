#include <iomanip>
#include <string>
#include <time.h>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    time_t time_seconds(seconds);
    tm *p = gmtime(&time_seconds);
    char result[36];
    sprintf(result, "%02d:%02d:%02d", p->tm_hour, p->tm_min, p->tm_sec);
    return result;
}