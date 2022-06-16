
#include "include/hal/esp32-c3/esp32_c3_Time.h"
#include <time.h>
#include <lwip/sockets.h>
#include "nvs_flash.h"
#include "nvs.h"

using namespace PocuterLib::HAL;

esp32_c3_Time::esp32_c3_Time() {
    m_currentTimeZone[0] = 0;
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
          ESP_ERROR_CHECK(nvs_flash_erase());
          err = nvs_flash_init();
    }
    if (err == ESP_OK) {
        nvs_handle_t nvsHandle;
        esp_err_t err = nvs_open("storage", NVS_READONLY, &nvsHandle);
        if (err == ESP_OK) {
            size_t size = 32;
            char tzString[32];
            err = nvs_get_str(nvsHandle, "TZ", tzString, &size);
            if (err == ESP_OK) {
                setenv("TZ",tzString,1);
                tzset();
                size = 32;
                err = nvs_get_str(nvsHandle, "TZN", m_currentTimeZone, &size);
            } else {
                strncpy(m_currentTimeZone, "GMT", 32);
            }
            
            nvs_close(nvsHandle);
        }
    }
}

esp32_c3_Time::~esp32_c3_Time() {
}
const char* esp32_c3_Time::getCurrentTimezone() {
    return m_currentTimeZone;
}
PocuterTime::TIMEERROR esp32_c3_Time::setTimezone(const pocuterTimezone* timeZ, bool save) {
    setenv("TZ",timeZ->timezoneString,1);
    tzset();
    if (save) {
        nvs_handle_t nvsHandle;
        esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvsHandle);
        if (err == ESP_OK) {
            err = nvs_set_str(nvsHandle, "TZ", timeZ->timezoneString);
            err = nvs_set_str(nvsHandle, "TZN", timeZ->name);
            if (err == ESP_OK) {
                strncpy(m_currentTimeZone, timeZ->name, 32);
            }

            nvs_close(nvsHandle);
        }
    }
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