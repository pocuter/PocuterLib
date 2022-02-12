
#include "include/hal/esp32-c3/esp32_c3_OTA.h"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_ota_ops.h"
#define FILE_BUFFER_SIZE 64

using namespace PocuterLib::HAL;

esp32_c3_OTA::esp32_c3_OTA(PocuterSDCard* SDCard) {
    m_SDCard = SDCard;
}


esp32_c3_OTA::~esp32_c3_OTA() {
}

PocuterOTA::OTAERROR esp32_c3_OTA::flashFromSDCard(uint64_t appID, POCUTER_PARTITION partition) {
    if (! m_SDCard->cardIsMounted()) return OTAERROR_NO_SD_CARD;
    vTaskDelay(configTICK_RATE_HZ / 100); // why I have to do this? Problems with SPI bus?
    esp_err_t err;
    PocuterOTA::OTAERROR pError = OTAERROR_OK;
    esp_ota_handle_t update_handle = 0; // must be freed!
    const esp_partition_t *running = esp_ota_get_running_partition();
    const esp_partition_t *update_partition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, (esp_partition_subtype_t)(ESP_PARTITION_SUBTYPE_APP_OTA_MIN + partition), NULL);
    if (update_partition == NULL) return OTAERROR_PATITION_NOT_AVAILABLE;
    if (running == update_partition) return OTAERROR_PATITION_IN_USE;
    char fileName[64];
    snprintf(fileName, 64, "%s/apps/%llu/%s", m_SDCard->getMountPoint(), appID, "/esp32c3.bin");
    FILE * fp = fopen (fileName, "rb");
    if (! fp) return OTAERROR_FILE_NOT_FOUND;
    // TODO: Check the File! CRC check and possibly Sining check?
    
    err = esp_ota_begin(update_partition, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);
    if (err != ESP_OK) return OTAERROR_UNKNOWN;
    
    size_t s = 0;
    size_t overall = 0;
    char buffer[FILE_BUFFER_SIZE];
    do {
        s = fread(buffer,1,FILE_BUFFER_SIZE,fp);
        if (s > 0) {
            overall += s;
            
            err = esp_ota_write( update_handle, (const void *)buffer, s);
            if (err != ESP_OK) {
                pError = OTAERROR_FLASHING_FAILED;
                break;
            }
        }
    }while (s == FILE_BUFFER_SIZE);
    fclose(fp);
    
    err = esp_ota_end(update_handle);
    if (err != ESP_OK) pError = OTAERROR_FLASHING_FAILED;
    
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