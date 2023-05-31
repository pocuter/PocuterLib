
#ifndef POCUTER_DISABLE_WIFI
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
#include <esp_http_server.h>
#include <string.h>
#include "include/hal/esp32-c3/esp32_c3_CaptivePortalDNS.h"

namespace PocuterLib {
    namespace HAL {
       
        class esp32_c3_WIFI : public PocuterWIFI {
        public:
            esp32_c3_WIFI();
            virtual ~esp32_c3_WIFI();
            
            void registerEventHandler(PocuterWIFI::wifiEventHandler*, void*);
            WIFIERROR startWPS();
            WIFIERROR startAccessPoint();
            WIFIERROR connect();
            WIFIERROR connect(const wifiCredentials*);
            WIFIERROR scanAPs(apInfo*, uint16_t* size, uint16_t* totalAPs);
            
            WIFIERROR getCredentials(PocuterWIFI::wifiCredentials*);
            WIFI_STATE getState();
            const ipInfo* getIpInfo();
            
            static esp_err_t http_get_handler(httpd_req_t *req);
            static esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err);
            
        private:
            
            PocuterWIFI::WIFIERROR wifiInit();
            PocuterWIFI::WIFIERROR wifiDeInit();
            static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
            static void got_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
            esp_netif_t *m_sta_netif;
            void start_webserver();
            void stop_webserver();
            
            WIFIERROR saveConfigOnSDCard(wifi_config_t *conf);
            WIFIERROR loadConfigFromSDCard(wifi_config_t *conf);
            
            static void wifiTask(void *arg);
            SemaphoreHandle_t m_wifiSemaphore;
            esp_wps_config_t m_wpsConfig;
            int m_RetryNum = 0;
            WIFI_STATE m_state;
            wifiEventHandler* m_wifiEventHandler;
            void* m_wifiEventHandler_userData;
            httpd_handle_t m_httpServer = NULL;
            bool m_didWifiInit;
            
            esp32_c3_CaptivePortalDNS* m_dns;
            const char* m_head =    "<!DOCTYPE HTML><html><head><meta content=\"text/html; charset=ISO-8859-1\"http-equiv=\"content-type\"><meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\"><title>Pocuter Portal</title><style>body { background-color: #0067B3  ; font-family: Arial, Helvetica, Sans-Serif; Color: #FFFFFF; }</style></head><body><center><h1 style=\"color:#FFFFFF; fontfamily:verdana;font-family: verdana;padding-top: 10px;padding-bottom: 10px;font-size: 36px\">Pocuter Portal</h1>";
            const char* m_webPageFormInput = "<h3 style=\"color:#FFFFFF;font-family: Verdana;font: caption;font-size: 27px;padding-top: 10px;padding-bottom: 10px;\">Please enter Your WiFi Credentials</h3><FORM action=\"/\" method=\"get\"><P ><label style=\"font-family:Times New Roman\">SSID</label><br><input maxlength=\"30px\" type='text'id=\"ssid_wifi\" name=\"ssid\" placeholder='Enter WiFi SSID' style=\"width: 300px; padding: 5px 10px ; margin: 8px 0; border : 2px solid #3498db; border-radius: 4px; box-sizing:border-box\" ><br></P><P><label style=\"font-family:Times New Roman\">PASSKEY</label><br><input maxlength=\"30px\" type = \"text\" id=\"pass_wifi\" name=\"passkey\"  placeholder = \"Enter WiFi PASSKEY\" style=\"width: 300px;padding: 5px 10px ; margin: 8px 0; border : 2px solid #3498db; border-radius: 4px; box-sizing:border-box\"><br>&nbsp;<p/><INPUT type=\"submit\" value = \"Submit\"><style>input[type=\"submit\"]{background-color: #3498DB; border: none; color: white; padding:  15px 48px;text-align: center; text-decoration: none;display: inline-block;font-size: 16px;}</style></FORM>";
            const char* m_webPageForm1 = "<h3 style=\"color:#FFFFFF;font-family: Verdana;font: caption;font-size: 27px;padding-top: 10px;padding-bottom: 10px;\">Please enter Your WiFi Credentials</h3><FORM action=\"/\" method=\"get\"><P ><label style=\"font-family:Times New Roman\">SSID</label><br><select maxlength=\"30px\" id=\"ssid_wifi\" name=\"ssid\" style=\"width: 300px; padding: 5px 10px ; margin: 8px 0; border : 2px solid #3498db; border-radius: 4px; box-sizing:border-box\" >";
            const char* m_webPageForm2 = "<option value = \"dfsf34fq434fq44\">Enter SSID...</option></select> <br></P><P><label style=\"font-family:Times New Roman\">PASSKEY</label><br><input maxlength=\"30px\" type = \"text\" id=\"pass_wifi\" name=\"passkey\"  placeholder = \"Enter WiFi PASSKEY\" style=\"width: 300px;padding: 5px 10px ; margin: 8px 0; border : 2px solid #3498db; border-radius: 4px; box-sizing:border-box\"><br>&nbsp;<p/><INPUT type=\"submit\" value = \"Submit\"><style>input[type=\"submit\"]{background-color: #3498DB; border: none; color: white; padding:  15px 48px;text-align: center; text-decoration: none;display: inline-block;font-size: 16px;}</style></FORM>";
            const char* m_webPageConnecting = "<h3 style=\"color:#FFFFFF;font-family: Verdana;font: caption;font-size: 27px;padding-top: 10px;padding-bottom: 10px;\">Connecting to Wifi</h3>";
            const char* m_foot =    "</center></body></html>";
            
            apInfo* m_apInfos; 
            uint16_t m_apInfosSize;
            
            static ipInfo s_info;
          
            
        };
    }
}

#endif /* ESP32_C3_WIFI_H */
#endif
