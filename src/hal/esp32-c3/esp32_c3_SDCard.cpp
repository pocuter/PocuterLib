#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_SD_CARD  
#include "include/hal/esp32-c3/esp32_c3_SDCard.h"

#include "esp_vfs_fat.h"
#include <string.h>
#include "include/hal/esp32-c3/esp32_c3_Expander.h"
#include "include/hal/PocuterDeviceType.h"
using namespace PocuterLib::HAL;


#define SD_PIN_NUM_CS      21
#define SD_PORT_CARDDETECT  0
#define SD_PIN_CARDDETECT   4

#define SD_PIN_NUM_CS_POCKET      10
#define SD_PORT_CARDDETECT_POCKET  0
#define SD_PIN_CARDDETECT_POCKET   3


#define MOUNT_POUNT "/sd" 
esp32_c3_SDCard::esp32_c3_SDCard() {
  
    m_sdcardMounted = false;
    
    if (PocuterDeviceType::deviceType == PocuterDeviceType::DEVICE_TYPE_POCUTER_1) {
        esp32_c3_Expander::Instance()->pinMode(SD_PORT_CARDDETECT, SD_PIN_CARDDETECT, EXPANDER_IN);
    } else {
        esp32_c3_Expander::Instance()->pinMode(SD_PORT_CARDDETECT_POCKET, SD_PIN_CARDDETECT_POCKET, EXPANDER_IN);
    }
    
    if (! cardInSlot()){
        return;
    }
    
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI2_HOST;
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    
    if (PocuterDeviceType::deviceType == PocuterDeviceType::DEVICE_TYPE_POCUTER_1) {
        slot_config.gpio_cs = (gpio_num_t)SD_PIN_NUM_CS;
    } else {
        slot_config.gpio_cs = (gpio_num_t)SD_PIN_NUM_CS_POCKET;
    }
    
    
    slot_config.host_id = SPI2_HOST;
    
    esp_vfs_fat_sdmmc_mount_config_t mount_config;
    memset(&mount_config, 0, sizeof(mount_config));
    mount_config.format_if_mount_failed = false;
    mount_config.max_files = 5;
    
    
    esp_err_t err = esp_vfs_fat_sdspi_mount(MOUNT_POUNT, &host, &slot_config, &mount_config, &m_card);
    if (err == ESP_OK) m_sdcardMounted = true;
   
}

esp32_c3_SDCard::~esp32_c3_SDCard() {
    if (m_sdcardMounted) {
        esp_vfs_fat_sdcard_unmount(MOUNT_POUNT, m_card);
    }
}

bool esp32_c3_SDCard::cardInSlot() {
    if (PocuterDeviceType::deviceType == PocuterDeviceType::DEVICE_TYPE_POCUTER_1) {
        return (! esp32_c3_Expander::Instance()->readPin(SD_PORT_CARDDETECT, SD_PIN_CARDDETECT));
    } else {
        return (! esp32_c3_Expander::Instance()->readPin(SD_PORT_CARDDETECT_POCKET, SD_PIN_CARDDETECT_POCKET));
    }
}
bool esp32_c3_SDCard::cardIsMounted() {
    return m_sdcardMounted;
}
const char* esp32_c3_SDCard::getMountPoint() {
    return MOUNT_POUNT;
}
#endif
