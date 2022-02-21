

#include "include/hal/esp32-c3/esp32_c3_WIFI.h"
#define MAX_RETRY_ATTEMPTS     2
#define TAG "WIFI_TEST"
#ifndef PIN2STR
#define PIN2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5], (a)[6], (a)[7]
#define PINSTR "%c%c%c%c%c%c%c%c"
#endif

using namespace PocuterLib::HAL;

esp32_c3_WIFI::esp32_c3_WIFI() {
    /* Initialize NVS — it is used to store PHY calibration data */
    m_state = WIFI_STATE_INIT_FAILED;
    m_wifiEventHandler = NULL;
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ret = nvs_flash_erase();
        if (ret == ESP_OK) {
            ret = nvs_flash_init();
        }
        
    }
    ret = esp_netif_init();
    if (ret != ESP_OK) return;
    ret = esp_event_loop_create_default();
    if (ret != ESP_OK) return;
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    if (!sta_netif) return;
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK) return;
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, this);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &got_ip_event_handler, this);
    
    
    m_state = WIFI_STATE_DISCONNECTED;
    
}

PocuterWIFI::WIFIERROR esp32_c3_WIFI::getCredentials(PocuterWIFI::wifiCredentials* c) {
    wifi_config_t conf;
    esp_err_t err = esp_wifi_get_config(WIFI_IF_STA, &conf);
    if (err != ESP_OK) return WIFIERROR_NO_CREDENTIALS;
    
    memcpy(c->ssid, conf.sta.ssid, sizeof(c->ssid));
    memcpy(c->password, conf.sta.password, sizeof(c->password));
    
    return WIFIERROR_OK;
    
}

PocuterWIFI::WIFI_STATE esp32_c3_WIFI::getState() {
    return m_state;
}

void esp32_c3_WIFI::wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    esp32_c3_WIFI* myself = (esp32_c3_WIFI*) arg;
    static int ap_idx = 1;
    switch (event_id) {
        case WIFI_EVENT_STA_CONNECTED:
            myself->m_state = WIFI_STATE_CONNECTED;
            if (myself->m_wifiEventHandler) myself->m_wifiEventHandler(WIFIEVENT_CONNECTED, NULL, myself->m_wifiEventHandler_userData);
            break;
        case WIFI_EVENT_STA_START:
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            if (myself->m_wifiEventHandler) myself->m_wifiEventHandler(WIFIEVENT_DISCONNECTED, NULL, myself->m_wifiEventHandler_userData);
            myself->m_state = WIFI_STATE_DISCONNECTED;
            if (myself->m_RetryNum < MAX_RETRY_ATTEMPTS) {
                esp_wifi_connect();
                myself->m_RetryNum++;
            } else if (ap_idx < myself->m_s_ap_creds_num) {
                /* Try the next AP credential if first one fails */

                if (ap_idx < myself->m_s_ap_creds_num) {
                    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &myself->m_wps_ap_creds[ap_idx++]) );
                    esp_wifi_connect();
                }
                myself->m_RetryNum = 0;
            } 

            break;
        case WIFI_EVENT_STA_WPS_ER_SUCCESS:
            {
                wifi_event_sta_wps_er_success_t *evt =
                    (wifi_event_sta_wps_er_success_t *)event_data;
                int i;
                if (evt) {
                    myself->m_s_ap_creds_num = evt->ap_cred_cnt;
                    for (i = 0; i < myself->m_s_ap_creds_num; i++) {
                        memcpy(myself->m_wps_ap_creds[i].sta.ssid, evt->ap_cred[i].ssid,
                               sizeof(evt->ap_cred[i].ssid));
                        memcpy(myself->m_wps_ap_creds[i].sta.password, evt->ap_cred[i].passphrase,
                               sizeof(evt->ap_cred[i].passphrase));
                    }
                    /* If multiple AP credentials are received from WPS, connect with first one */
                    
                    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &myself->m_wps_ap_creds[0]) );
                }
                /*
                 * If only one AP credential is received from WPS, there will be no event data and
                 * esp_wifi_set_config() is already called by WPS modules for backward compatibility
                 * with legacy apps. So directly attempt connection here.
                 */
                ESP_ERROR_CHECK(esp_wifi_wps_disable());
                esp_wifi_connect();
            }
            break;
        case WIFI_EVENT_STA_WPS_ER_FAILED:
            ESP_ERROR_CHECK(esp_wifi_wps_disable());
            ESP_ERROR_CHECK(esp_wifi_wps_enable(&myself->m_wpsConfig));
            ESP_ERROR_CHECK(esp_wifi_wps_start(0));
            myself->m_state = WIFI_STATE_DISCONNECTED;
            break;
        case WIFI_EVENT_STA_WPS_ER_TIMEOUT:
            ESP_ERROR_CHECK(esp_wifi_wps_disable());
            ESP_ERROR_CHECK(esp_wifi_wps_enable(&myself->m_wpsConfig));
            ESP_ERROR_CHECK(esp_wifi_wps_start(0));
            myself->m_state = WIFI_STATE_DISCONNECTED;
            break;
        case WIFI_EVENT_STA_WPS_ER_PIN:
            {
                /* display the PIN code */
                wifi_event_sta_wps_er_pin_t* event = (wifi_event_sta_wps_er_pin_t*) event_data;
                //printf("WPS_PIN = " PINSTR "\n", PIN2STR(event->pin_code));
                break;
            }
        default:
            break;
    }
    
}
void esp32_c3_WIFI::got_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    static ipInfo info;
    esp32_c3_WIFI* myself = (esp32_c3_WIFI*) arg;
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    info.ipV4 = (uint32_t)event->ip_info.ip.addr;
    info.gw = event->ip_info.gw.addr;
    info.netmask = event->ip_info.netmask.addr;
    
    if (myself->m_wifiEventHandler) myself->m_wifiEventHandler(WIFIEVENT_GOT_IP, &info, myself->m_wifiEventHandler_userData);
   
}
void esp32_c3_WIFI::registerEventHandler(PocuterWIFI::wifiEventHandler* h, void* u) {
    m_wifiEventHandler = h;
    m_wifiEventHandler_userData = u;
}
PocuterWIFI::WIFIERROR esp32_c3_WIFI::connect(const PocuterWIFI::wifiCredentials* c) {
    esp_err_t ret = esp_wifi_set_mode(WIFI_MODE_STA);
    if (ret != ESP_OK) return WIFIERROR_COULD_NOT_SET_WIFI_MODE;
    wifi_config_t wifi_config = {
        .sta = {
            
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	//     .threshold.authmode = WIFI_AUTH_WPA2_PSK,

            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    strncpy((char*)wifi_config.sta.ssid, (char*)c->ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char*)wifi_config.sta.password, (char*)c->password, sizeof(wifi_config.sta.password));
    if (esp_wifi_set_config(WIFI_IF_STA, &wifi_config) != ESP_OK) return WIFIERROR_INIT_FAILED;
    ret = esp_wifi_start();
    if (ret != ESP_OK) return WIFIERROR_INIT_FAILED;
    ret = esp_wifi_connect();
    if (ret != ESP_OK) return WIFIERROR_INIT_FAILED;
    return WIFIERROR_OK;
}
PocuterWIFI::WIFIERROR esp32_c3_WIFI::connect() {
    esp_err_t ret = esp_wifi_set_mode(WIFI_MODE_STA);
    if (ret != ESP_OK) return WIFIERROR_COULD_NOT_SET_WIFI_MODE;
    ret = esp_wifi_start();
    if (ret != ESP_OK) return WIFIERROR_INIT_FAILED;
    ret = esp_wifi_connect();
    if (ret != ESP_OK) return WIFIERROR_INIT_FAILED;
    
    return WIFIERROR_OK;
}
PocuterWIFI::WIFIERROR esp32_c3_WIFI::startWPS() {
    esp_err_t ret;
    m_s_ap_creds_num = 0;
    m_RetryNum = 0;
    m_wpsConfig.wps_type = WPS_TYPE_PBC;
    m_state = WIFI_WAITING_WPS;
    
    strncpy(m_wpsConfig.factory_info.device_name, "POCUTER", 33);
    strncpy(m_wpsConfig.factory_info.manufacturer, "POCUTER GMBH", 33);
    strncpy(m_wpsConfig.factory_info.model_name, "POCUTER 1", 33);
    strncpy(m_wpsConfig.factory_info.model_number, "ONE", 33);
    
    esp_wifi_disconnect();
    
    ret = esp_wifi_set_mode(WIFI_MODE_STA);
    if (ret != ESP_OK) return WIFIERROR_COULD_NOT_SET_WIFI_MODE;
    ret = esp_wifi_start();
    if (ret != ESP_OK) return WIFIERROR_INIT_FAILED;
    esp_wifi_wps_enable(&m_wpsConfig);
    esp_wifi_wps_start(0);
    return WIFIERROR_OK;
}

esp32_c3_WIFI::~esp32_c3_WIFI() {
    esp_wifi_stop();
    esp_wifi_deinit();
}

