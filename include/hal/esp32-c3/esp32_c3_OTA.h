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
            OTAERROR getApps(std::vector<uint64_t>* apps, int maxLength, int offset);
            uint32_t getAppsCount();
            
            OTAERROR getAppVersion(uint64_t appID, uint8_t* major, uint8_t* minor, uint8_t* patch);
            OTAERROR flashFromSDCard(uint64_t appID, POCUTER_PARTITION partition, bool checkSigning, bool stepwise = false, uint8_t *percent = NULL);
            OTAERROR bootPartition(POCUTER_PARTITION partition);
            POCUTER_PARTITION getCurrentPartition();
            OTAERROR restart();
            
            static POCUTER_PARTITION getCurrentPartitionStatic();
            static uint64_t getCurrentAppID();
            
            virtual ~esp32_c3_OTA();
            
            struct fheader {
                char magic[5];
                uint8_t fileVersion;
                uint16_t crc16;
                uint64_t appID;
                struct version {
                    uint8_t major;
                    uint8_t minor;
                    uint8_t patch;
                } version;
                struct features {
                    char hasSignartur : 1;
                    char hasMetaData  : 1;
                    char forFutureUse : 6;
                } features;
                uint32_t startSignatur;
                uint32_t sizeSignatur;
                uint32_t startMetaFile;
                uint32_t sizeMetaFile;
                uint32_t startFlashFile;
                uint32_t sizeFlashFile;

            };

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

