
#ifndef ESP32_C3_ADC_H
#define ESP32_C3_ADC_H

#include "driver/adc.h"

namespace PocuterLib {
    namespace HAL {
        class esp32_c3_ADC {
        public:
            esp32_c3_ADC();
            static int singleRead(adc1_channel_t);
            virtual ~esp32_c3_ADC();
        private:
            
        };
    }
}

#endif /* ESP32_C3_ADC_H */

