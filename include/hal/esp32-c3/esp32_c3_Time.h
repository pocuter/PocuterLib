
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
            
            TIMEERROR setTimezone(const char *timezoneString);
            TIMEERROR getLocalTime(tm*);
            TIMEERROR getGMTTime(tm*);
            TIMEERROR setLocalTime(tm*);
       
        private:

        };
    }
}

#endif /* ESP32_C3_TIME_H */

