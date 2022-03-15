#ifndef ESP32_C3_OTA_H
#define ESP32_C3_OTA_H
#include "include/hal/PocuterSDCard.h"
#include "include/hal/PocuterOTA.h"
#include "esp_ota_ops.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
namespace PocuterLib {
    namespace HAL {
        class esp32_c3_OTA : public PocuterOTA {
        public:
            esp32_c3_OTA(PocuterSDCard* SDCard);
            OTAERROR setNextAppID(uint64_t appID);
            OTAERROR flashFromSDCard(uint64_t appID, POCUTER_PARTITION partition, bool checkSigning, bool stepwise = false, uint8_t *percent = NULL);
            OTAERROR bootPartition(POCUTER_PARTITION partition);
            POCUTER_PARTITION getCurrentPartition();
            OTAERROR restart();
            virtual ~esp32_c3_OTA();
        private:
            PocuterSDCard* m_SDCard;
            char* m_buffer;
            char* m_deflatebuffer;
            FILE * m_fp;
            size_t m_overall;
            size_t m_fileSize;
            esp_ota_handle_t m_update_handle;
            const esp_partition_t *m_update_partition;

        };
    }
}

#endif /* ESP32_C3_OTA_H */

