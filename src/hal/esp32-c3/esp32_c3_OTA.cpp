#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_SD_CARD
#include "include/hal/esp32-c3/esp32_c3_OTA.h"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#define FILE_BUFFER_SIZE 1024*16

using namespace PocuterLib::HAL;

esp32_c3_OTA::esp32_c3_OTA(PocuterSDCard* SDCard) {
    m_SDCard = SDCard;
    m_buffer = NULL;
    m_fp = NULL;
}


esp32_c3_OTA::~esp32_c3_OTA() {
}

PocuterOTA::OTAERROR esp32_c3_OTA::flashFromSDCard(uint64_t appID, POCUTER_PARTITION partition, bool stepwise, uint8_t *percent) {
    if (! m_SDCard->cardIsMounted()) return OTAERROR_NO_SD_CARD;
    esp_err_t err;
    PocuterOTA::OTAERROR pError = OTAERROR_OK;
    if (stepwise) pError = OTAERROR_MORE_STEPS;
    if (! m_fp){
        m_update_handle = 0; // must be freed!
        const esp_partition_t *running = esp_ota_get_running_partition();
        m_update_partition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, (esp_partition_subtype_t)(ESP_PARTITION_SUBTYPE_APP_OTA_MIN + partition), NULL);
        if (m_update_partition == NULL) return OTAERROR_PATITION_NOT_AVAILABLE;
        if (running == m_update_partition) return OTAERROR_PATITION_IN_USE;
        char fileName[64];
        snprintf(fileName, 64, "%s/apps/%llu/%s", m_SDCard->getMountPoint(), appID, "/esp32c3.bin");
    
        m_overall = 0;
        m_fp = fopen (fileName, "rb");
        if (m_fp) {
            fseek(m_fp, 0L, SEEK_END);
            m_fileSize = ftell(m_fp);
            fseek(m_fp, 0L, SEEK_SET);
        } else {
            return OTAERROR_FILE_NOT_FOUND;
        }
        err = esp_ota_begin(m_update_partition, OTA_WITH_SEQUENTIAL_WRITES, &m_update_handle);
        if (err != ESP_OK) return OTAERROR_UNKNOWN;
    }
    
    // TODO: Check the File! CRC check and possibly Sining check?
    
    
    
    size_t s = 0;
    if (m_buffer == NULL) m_buffer = (char*)malloc(FILE_BUFFER_SIZE);
    do {
        s = fread(m_buffer,1,FILE_BUFFER_SIZE,m_fp);
        if (s > 0) {
            m_overall += s;
            
            err = esp_ota_write( m_update_handle, (const void *)m_buffer, s);
            if (err != ESP_OK) {
                pError = OTAERROR_FLASHING_FAILED;
                break;
            }
        }
    }while (s == FILE_BUFFER_SIZE && stepwise == false);
    if (percent != NULL) {
        if (m_fileSize > 0) *percent = m_overall * 100 / m_fileSize; else *percent = 100;
    }
    if ( s != FILE_BUFFER_SIZE || pError == OTAERROR_FLASHING_FAILED) {
        err = esp_ota_end(m_update_handle);
        free(m_buffer);
        m_buffer = NULL;
        fclose(m_fp);
        m_fp = NULL;
        if (pError == OTAERROR_MORE_STEPS) pError = OTAERROR_OK;
        if (err != ESP_OK) {
            pError = OTAERROR_FLASHING_FAILED;
        } else {
           err = esp_ota_set_boot_partition(m_update_partition);
           if (err != ESP_OK) pError = OTAERROR_FLASHING_FAILED;
        }
    }
    
    return pError;
    
}
PocuterOTA::OTAERROR esp32_c3_OTA::bootPartition(PocuterOTA::POCUTER_PARTITION partition) {
    const esp_partition_t *bootPartition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, (esp_partition_subtype_t)(ESP_PARTITION_SUBTYPE_APP_OTA_MIN + partition), NULL);
    if (bootPartition == NULL) return OTAERROR_PATITION_NOT_AVAILABLE;
    // ToDO check valid Partition?
    esp_err_t err = esp_ota_set_boot_partition(bootPartition);
    if (err != ESP_OK) return OTAERROR_UNKNOWN;
    return OTAERROR_OK;
}
PocuterOTA::POCUTER_PARTITION esp32_c3_OTA::getCurrentPartition() {
    const esp_partition_t *running = esp_ota_get_running_partition();
    if (running != NULL && running->type == ESP_PARTITION_TYPE_APP) {
        if (running->subtype >= ESP_PARTITION_SUBTYPE_APP_OTA_MIN && running->subtype < ESP_PARTITION_SUBTYPE_APP_OTA_MAX){
            return (POCUTER_PARTITION)(running->subtype - ESP_PARTITION_SUBTYPE_APP_OTA_MIN);
        }
    }
    return PART_UNKNOWN;
    
}
PocuterOTA::OTAERROR esp32_c3_OTA::restart() {
    esp_restart();
    return OTAERROR_UNKNOWN;
}
#endif