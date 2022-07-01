#ifndef POCUTER_DISABLE_TIME
#include "include/hal/esp32-c3/esp32_c3_Time.h"
#include "include/hal/PocuterConfig.h"
#include <esp_sntp.h>
#include <time.h>
#include <lwip/sockets.h>
#include "nvs_flash.h"
#include "nvs.h"

#define DEFAULT_SNTPSERVER "pool.ntp.org"
using namespace PocuterLib::HAL;

esp32_c3_Time::esp32_c3_Time() {
    m_currentTimeZone[0] = 0;
    char buffer[32];
    uint64_t appId = 1;
    PocuterConfig* config = new PocuterConfig((const uint8_t*)"TIME", &appId);
    bool gotN = config->get((const uint8_t*)"TIMEZONE", (const uint8_t*)"NAME", (uint8_t*)m_currentTimeZone, 32);  
    bool gotS = config->get((const uint8_t*)"TIMEZONE", (const uint8_t*)"STRING", (uint8_t*)buffer, 32);  
    if (gotN && gotS) {
        setenv("TZ",buffer,1);
    }
    if (! config->get((const uint8_t*)"SERVER", (const uint8_t*)"NAME", (uint8_t*)m_currentSNTPServerName, 64)) {
        strncpy(m_currentSNTPServerName, DEFAULT_SNTPSERVER, 64);
    }
     m_isSNTPOn = true;
    if (config->get((const uint8_t*)"SERVER", (const uint8_t*)"ACTIVE", (uint8_t*)buffer, 32)) {
        if (strncmp(buffer, "ON", 2))  m_isSNTPOn = false;
    }
    if (m_isSNTPOn){
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, m_currentSNTPServerName);
        sntp_init();
    }
    delete(config);
    
}

esp32_c3_Time::~esp32_c3_Time() {
}
const char* esp32_c3_Time::getCurrentTimezone() {
    return m_currentTimeZone;
}
PocuterTime::TIMEERROR esp32_c3_Time::setTimezone(const pocuterTimezone* timeZ, bool save) {
    setenv("TZ",timeZ->timezoneString,1);
    tzset();
    TIMEERROR ret = TIMEERROR_FAILED;
    if (save) {
        nvs_handle_t nvsHandle;
        uint64_t appId = 1;
        PocuterConfig* config = new PocuterConfig((const uint8_t*)"TIME", &appId);
        if (config != NULL) {
            bool nsaved = config->set((const uint8_t*)"TIMEZONE", (const uint8_t*)"NAME", (const uint8_t*)timeZ->name);
            bool ssaved = config->set((const uint8_t*)"TIMEZONE", (const uint8_t*)"STRING", (const uint8_t*)timeZ->timezoneString);
            if (nsaved && ssaved) {
                strncpy(m_currentTimeZone, timeZ->name, 32);
                ret = TIMEERROR_OK;
            }
            delete config;
        }
        
    } else {
        strncpy(m_currentTimeZone, timeZ->name, 32);
        ret = TIMEERROR_OK;
    }
    
    return ret;
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



PocuterTime::TIMEERROR esp32_c3_Time::setTimeServer(const char* timeServer) {
    TIMEERROR ret = TIMEERROR_FAILED;
    if (timeServer == NULL) return ret;
    strncpy(m_currentSNTPServerName, timeServer, 64);
    uint64_t appId = 1;
    PocuterConfig* config = new PocuterConfig((const uint8_t*)"TIME", &appId);
    if (config->set((const uint8_t*)"SERVER", (const uint8_t*)"NAME", (const uint8_t*)timeServer)) ret = TIMEERROR_OK;
    delete(config);
    
    if (isSNTP()) {
        sntp_stop();
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, timeServer);
        sntp_init();
    }
    return ret;
}
PocuterTime::TIMEERROR esp32_c3_Time::setSNTPD(bool on) {
    TIMEERROR ret = TIMEERROR_FAILED;
    if (m_isSNTPOn == on) return TIMEERROR_OK;
    m_isSNTPOn = on;
    uint64_t appId = 1;
    PocuterConfig* config = new PocuterConfig((const uint8_t*)"TIME", &appId);
    
    if (on) {
        if (config->set((const uint8_t*)"SERVER", (const uint8_t*)"ACTIVE", (const uint8_t*)"ON")) ret = TIMEERROR_OK;
    } else {
        if (config->set((const uint8_t*)"SERVER", (const uint8_t*)"ACTIVE", (const uint8_t*)"OFF")) ret = TIMEERROR_OK;
    }
    
    
    delete(config);
    
    if (isSNTP()) {
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, m_currentSNTPServerName);
        sntp_init();
    } else {
        sntp_stop();
    }
    return ret;
}
bool esp32_c3_Time::isSNTP(){
    return m_isSNTPOn;
}
const char* esp32_c3_Time::getSMTPTimeServer(){
    return m_currentSNTPServerName;
}
            
            
            
#endif
