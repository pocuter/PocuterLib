
#include "include/hal/esp32-c3/esp32_c3_Time.h"
#include <time.h>
#include <lwip/sockets.h>


using namespace PocuterLib::HAL;

esp32_c3_Time::esp32_c3_Time() {
}

esp32_c3_Time::~esp32_c3_Time() {
}

PocuterTime::TIMEERROR esp32_c3_Time::setTimezone(const char *timezoneString) {
    setenv("TZ",timezoneString,1);
    tzset();
    return TIMEERROR_OK;
}
PocuterTime::TIMEERROR esp32_c3_Time::getLocalTime(tm* localtime) {
    time_t now;
    time(&now);
    localtime_r(&now, localtime);
    return TIMEERROR_OK;
}
PocuterTime::TIMEERROR esp32_c3_Time::getGMTTime(tm* gtime){
    time_t now;
    time(&now);
    gmtime_r(&now, gtime);
    return TIMEERROR_OK;
}
PocuterTime::TIMEERROR esp32_c3_Time::setLocalTime(tm* localtime){
    localtime->tm_isdst = 0;
    tm cLocalTime = *localtime;
    
    time_t t = mktime(localtime);
    
    // check for dst;
    tm timeinfo; 
    localtime_r(&t, &timeinfo);
    
    if (cLocalTime.tm_isdst != timeinfo.tm_isdst){
        // we have dst
        cLocalTime.tm_isdst = timeinfo.tm_isdst;
        t = mktime(&cLocalTime);
    }
    
    
    timeval tv;
    tv.tv_sec = t;
    tv.tv_usec = 0;
    
    
    settimeofday(&tv, NULL);
   
    return TIMEERROR_OK;
}