
#ifndef ESP32_C3_BATTERY_H
#define ESP32_C3_BATTERY_H

#include "include/hal/PocuterBattery.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
namespace PocuterLib  {
    namespace HAL {
        class esp32_c3_Battery : public PocuterBattery {
        public:
            esp32_c3_Battery();
            virtual ~esp32_c3_Battery();
			
			
			uint16_t getBatteryLevel();
			bool isCharging();
            
        private:
        };
    }
}

#endif /* ESP32_C3_BATTERY_H */

