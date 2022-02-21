
#ifndef ESP32_C3_WIFI_H
#define ESP32_C3_WIFI_H
#include "include/hal/PocuterWIFI.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_wps.h"
#include "nvs_flash.h"
#include <string.h>
namespace PocuterLib {
    namespace HAL {
       
        class esp32_c3_WIFI : public PocuterWIFI {
        public:
            esp32_c3_WIFI();
            virtual ~esp32_c3_WIFI();
            
            void registerEventHandler(PocuterWIFI::wifiEventHandler*, void*);
            WIFIERROR startWPS();
            WIFIERROR connect();
            WIFIERROR connect(const wifiCredentials*);
            
            WIFIERROR getCredentials(PocuterWIFI::wifiCredentials*);
            WIFI_STATE getState();
        private:
            
            
            static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
            static void got_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
            esp_wps_config_t m_wpsConfig;
            wifi_config_t m_wps_ap_creds[MAX_WPS_AP_CRED];
            int m_s_ap_creds_num = 0;
            int m_RetryNum = 0;
            WIFI_STATE m_state;
            wifiEventHandler* m_wifiEventHandler;
            void* m_wifiEventHandler_userData;
        };
    }
}

#endif /* ESP32_C3_WIFI_H */

