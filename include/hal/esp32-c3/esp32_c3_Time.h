
#ifndef ESP32_C3_TIME_H
#define ESP32_C3_TIME_H

#include "include/hal/PocuterTime.h"
#include <time.h>

namespace PocuterLib  {
    namespace HAL {
        class esp32_c3_Time : public PocuterTime {
        public:
            esp32_c3_Time();
            virtual ~esp32_c3_Time();
            
            TIMEERROR setTimezone(const pocuterTimezone*, bool save = true);
            TIMEERROR getLocalTime(tm*);
            TIMEERROR getGMTTime(tm*);
            TIMEERROR setLocalTime(tm*);
            const char* getCurrentTimezone();
            
            TIMEERROR setTimeServer(const char* timeServer);
            TIMEERROR setSNTPD(bool on);
            bool isSNTP();
            const char* getSMTPTimeServer();
            
        private:
            char m_currentTimeZone[32];
            char m_currentSNTPServerName[64];
            bool m_isSNTPOn;
            
        };
    }
}

#endif /* ESP32_C3_TIME_H */

