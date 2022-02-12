#ifndef ESP32_C3_OTA_H
#define ESP32_C3_OTA_H
#include "include/hal/PocuterSDCard.h"
#include "include/hal/PocuterOTA.h"
#include <stdint.h>
namespace PocuterLib {
    namespace HAL {
        class esp32_c3_OTA : public PocuterOTA {
        public:
            esp32_c3_OTA(PocuterSDCard* SDCard);
            OTAERROR flashFromSDCard(uint64_t appID, POCUTER_PARTITION partition);
            OTAERROR bootPartition(POCUTER_PARTITION partition);
            POCUTER_PARTITION getCurrentPartition();
            OTAERROR restart();
            virtual ~esp32_c3_OTA();
        private:
            PocuterSDCard* m_SDCard;

        };
    }
}

#endif /* ESP32_C3_OTA_H */

