#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_SD_CARD
#include "include/hal/esp32-c3/esp32_c3_OTA.h"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "include/puff/puff.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <dirent.h> 

#define FILE_BUFFER_SIZE 1024*40
#define DEFLATE_BUFFER_SIZE 1024*40

using namespace PocuterLib::HAL;




esp32_c3_OTA::esp32_c3_OTA(PocuterSDCard* SDCard) {
    m_SDCard = SDCard;
    m_buffer = NULL;
    m_deflatebuffer = NULL;
    m_fp = NULL;
    
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
          // NVS partition was truncated and needs to be erased
          // Retry nvs_flash_init
          ESP_ERROR_CHECK(nvs_flash_erase());
          err = nvs_flash_init();
    }
    
}


esp32_c3_OTA::~esp32_c3_OTA() {
}
uint32_t esp32_c3_OTA::getAppsCount() {
    uint32_t c = 0;
    DIR *d;
    struct dirent *dir;
    char dirName[64];
    snprintf(dirName, 64, "%s/apps", m_SDCard->getMountPoint());
    d = opendir(dirName);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_DIR) {// 
                char* end;
                uint64_t appID = strtoull(dir->d_name, &end, 10);
                if (appID > 0) {
                    c++;
                }
            }
        }
        closedir(d);
    }
    return c;
}
PocuterOTA::OTAERROR esp32_c3_OTA::getApps(std::vector<uint64_t>* apps, int maxLength, int offset) {
    DIR *d;
    struct dirent *dir;
    char dirName[64];
    apps->clear();
    uint32_t currAppNum = 0;
    uint32_t currLimit = 0;
    snprintf(dirName, 64, "%s/apps", m_SDCard->getMountPoint());
    d = opendir(dirName);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            
            if (dir->d_type == DT_DIR) {// 
                
                char* end;
                uint64_t appID = strtoull(dir->d_name, &end, 10);
                if (appID > 0) {
                    if (offset <= currAppNum++) {
                        if (++currLimit > maxLength) break;
                        apps->push_back(appID);
                    }
                    
                }
                
            }
        }
        closedir(d);
    }
    if (apps->size() > 0) return OTAERROR_OK;
    return OTAERROR_APP_READ_ERROR;
}
PocuterOTA::OTAERROR esp32_c3_OTA::getAppVersion(uint64_t appID, uint8_t* major, uint8_t* minor, uint8_t* patch ) {
    char fileName[64];
    snprintf(fileName, 64, "%s/apps/%llu/%s", m_SDCard->getMountPoint(), appID, "/esp32c3.app");
    FILE* fp = fopen (fileName, "rb");
    OTAERROR err = OTAERROR_FILE_NOT_FOUND;
    if (fp) {
        fheader header;
        size_t s = fread(&header, sizeof(fheader), 1, fp);
        if (s > 0) {
            *major = header.version.major;
            *minor = header.version.minor;
            *patch = header.version.patch;
            err = OTAERROR_OK;
        } else {
            err = OTAERROR_APP_READ_ERROR;
        }
        fclose(fp);
    }
    return err;  
}
PocuterOTA::OTAERROR esp32_c3_OTA::setNextAppID(uint64_t appID) {
    
   
     nvs_handle_t nvsHandle;
   
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvsHandle);
    if (err == ESP_OK) {
        err = nvs_set_u64(nvsHandle, "startApp", appID);
        if (err == ESP_OK) {
            err = nvs_commit(nvsHandle);
        }
        nvs_close(nvsHandle);
      }
    
    if (err == ESP_OK){
        bootPartition(PocuterOTA::PART_APPLOADER);
        return OTAERROR_OK;
    }
    return OTAERROR_UNKNOWN;
}
PocuterOTA::OTAERROR esp32_c3_OTA::flashFromSDCard(uint64_t appID, POCUTER_PARTITION partition, bool checkSigning, bool stepwise, uint8_t *percent) {
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
        snprintf(fileName, 64, "%s/apps/%llu/%s", m_SDCard->getMountPoint(), appID, "/esp32c3.app");
        
        m_overall = 0;
        m_fp = fopen (fileName, "rb");
        if (m_fp) {
            // Read Header
            fheader header;
            fread(&header, sizeof(fheader), 1, m_fp);
            m_fileSize = header.sizeFlashFile;
            fseek(m_fp, header.startFlashFile, SEEK_SET);
        } else {
            return OTAERROR_FILE_NOT_FOUND;
        }
        err = esp_ota_begin(m_update_partition, OTA_WITH_SEQUENTIAL_WRITES, &m_update_handle);
        if (err != ESP_OK) return OTAERROR_UNKNOWN;
    }
        
    size_t s = 0;
    if (m_buffer == NULL) m_buffer = (char*)malloc(FILE_BUFFER_SIZE);
    if (m_deflatebuffer == NULL) m_deflatebuffer = (char*)malloc(DEFLATE_BUFFER_SIZE);
    do {
        s = fread(m_buffer,1,4,m_fp);
        if (s <= 0) break;
        uint32_t blockLength = *((uint32_t*)m_buffer);
        if (blockLength > FILE_BUFFER_SIZE || blockLength < 3) break;
        s = fread(m_buffer,1,2,m_fp); // skip header
        if (s <= 0) break;
        
        long unsigned int outBytes;
        s = fread(m_buffer,1,blockLength - 2,m_fp);
        outBytes = DEFLATE_BUFFER_SIZE;
        
        uint32_t infaltedIn = s;
        
        int status = puff((unsigned char*)m_deflatebuffer, &outBytes, (unsigned char*)m_buffer, &infaltedIn);
        if (status != 0) {
            return OTAERROR_FLASHING_FAILED;
        }
        
        m_overall += s + 6;
        
        if (outBytes > 0) {
            err = esp_ota_write( m_update_handle, (const void *)m_deflatebuffer, outBytes);
            if (err != ESP_OK) {
                pError = OTAERROR_FLASHING_FAILED;
                break;
            }
        }
    }while (stepwise == false);
    
    if (percent != NULL) {
        if (m_fileSize > 0) *percent = m_overall * 100 / m_fileSize; else *percent = 100;
    }
    if ( m_overall >= m_fileSize || pError == OTAERROR_FLASHING_FAILED) {
        err = esp_ota_end(m_update_handle);
        free(m_buffer);
        m_buffer = NULL;
        free(m_deflatebuffer);
        m_deflatebuffer = NULL;
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
    esp_err_t err = esp_ota_set_boot_partition(bootPartition);
    if (err != ESP_OK) return OTAERROR_UNKNOWN;
    return OTAERROR_OK;
}
PocuterOTA::POCUTER_PARTITION esp32_c3_OTA::getCurrentPartition() {
    return getCurrentPartitionStatic();
}
PocuterOTA::POCUTER_PARTITION esp32_c3_OTA::getCurrentPartitionStatic() {
    const esp_partition_t *running = esp_ota_get_running_partition();
    if (running != NULL && running->type == ESP_PARTITION_TYPE_APP) {
        if (running->subtype >= ESP_PARTITION_SUBTYPE_APP_OTA_MIN && running->subtype < ESP_PARTITION_SUBTYPE_APP_OTA_MAX){
            return (POCUTER_PARTITION)(running->subtype - ESP_PARTITION_SUBTYPE_APP_OTA_MIN);
        }
    }
    return PART_UNKNOWN;
    
}
PocuterOTA::OTAERROR esp32_c3_OTA::verifyPartition(POCUTER_PARTITION partition) {
    esp_image_metadata_t dat;
    const esp_partition_t *bootPartition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, (esp_partition_subtype_t)(ESP_PARTITION_SUBTYPE_APP_OTA_MIN + partition), NULL);
    if (bootPartition == NULL) return OTAERROR_PATITION_NOT_AVAILABLE;
    esp_partition_pos_t part; 
    part.offset = bootPartition->address;
    part.size = bootPartition->size;
    dat.start_addr = bootPartition->address;
    if (esp_image_verify(ESP_IMAGE_VERIFY_SILENT, &part, &dat) == ESP_OK) return OTAERROR_OK;
    return OTAERROR_APP_READ_ERROR;
    
      
}
PocuterOTA::OTAERROR esp32_c3_OTA::restart() {
    esp_restart();
    return OTAERROR_UNKNOWN;
}
uint64_t esp32_c3_OTA::getCurrentAppID() {
    PocuterOTA::POCUTER_PARTITION part = getCurrentPartitionStatic();
    if (part == PART_APPLOADER) return 1;
    if (part == PART_UNKNOWN) return 0;
    
    uint64_t currentAPP = 1;  
    nvs_handle_t nvsHandle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvsHandle);
    if (err == ESP_OK) {
        err = nvs_get_u64(nvsHandle, "startApp", &currentAPP);
        nvs_close(nvsHandle);
    }
    return currentAPP;
}


#endif