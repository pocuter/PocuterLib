#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_WIFI

#include "include/hal/esp32-c3/esp32_c3_WIFI.h"
#include "include/hal/esp32-c3/esp32_c3_CaptivePortalDNS.h"
#include "include/hal/PocuterConfig.h"
#define MAX_RETRY_ATTEMPTS     2
#define TAG "WIFI_TEST"
#ifndef PIN2STR
#define PIN2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5], (a)[6], (a)[7]
#define PINSTR "%c%c%c%c%c%c%c%c"
#endif

#define ESP_WIFI_SSID "POCUTER"

#pragma GCC diagnostic push     
#pragma GCC diagnostic ignored "-Wmissing-field-initializers" 


using namespace PocuterLib::HAL;


static httpd_uri_t uri_get = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = esp32_c3_WIFI::http_get_handler,
    .user_ctx  = NULL
};



esp32_c3_WIFI::esp32_c3_WIFI() {
    /* Initialize NVS — it is used to store PHY calibration data */
    m_didWifiInit = false;
    m_sta_netif = NULL;
    m_state = WIFI_STATE_INIT_FAILED;
    m_wifiEventHandler = NULL;
    m_dns = NULL;
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
    
    
    m_state = WIFI_STATE_DISCONNECTED;
    
}
PocuterWIFI::WIFIERROR esp32_c3_WIFI::wifiInit() {
    if (m_didWifiInit) return WIFIERROR_OK;
    esp_err_t ret;
    m_sta_netif = esp_netif_create_default_wifi_sta();
    if (!m_sta_netif) return WIFIERROR_INIT_FAILED;
    
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    
    
    ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK) return WIFIERROR_INIT_FAILED;
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, this);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &got_ip_event_handler, this);
    m_didWifiInit = true;
    return WIFIERROR_OK;
}
PocuterWIFI::WIFIERROR esp32_c3_WIFI::wifiDeInit() {
    if (! m_didWifiInit) return WIFIERROR_OK; 
    esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler);
    esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &got_ip_event_handler);
    esp_wifi_disconnect();
    esp_wifi_stop();
    if (m_sta_netif){
        esp_netif_destroy(m_sta_netif);
        m_sta_netif = NULL;
    }
    esp_wifi_deinit();
    m_didWifiInit = false;
    vTaskDelay(500 / portTICK_PERIOD_MS);
    return WIFIERROR_OK;
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

PocuterWIFI::WIFIERROR esp32_c3_WIFI::saveConfigOnSDCard(wifi_config_t *conf) {
    PocuterConfig* config = new PocuterConfig(1);
    bool ok = config->setBinary((const uint8_t*)"WIFI", (const uint8_t*)"config", conf, sizeof(wifi_config_t));
    delete(config);
    if (ok)  return WIFIERROR_OK;
    return WIFIERROR_UNKNOWN;
}
PocuterWIFI::WIFIERROR esp32_c3_WIFI::loadConfigFromSDCard(wifi_config_t *conf){
    PocuterConfig* config = new PocuterConfig(1);
    bool ok = config->getBinary((const uint8_t*)"WIFI", (const uint8_t*)"config", conf, sizeof(wifi_config_t));
    delete(config);
    if (ok)  return WIFIERROR_OK;
    return WIFIERROR_UNKNOWN;
}
            
void esp32_c3_WIFI::wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    esp32_c3_WIFI* myself = (esp32_c3_WIFI*) arg;
    printf("EVENT: %d\n", event_id);
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
                    if (myself->saveConfigOnSDCard(&myself->m_wps_ap_creds[0]) != WIFIERROR_OK) {
                        printf("Could not save config on SD Card!\n");
                    }
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
                //wifi_event_sta_wps_er_pin_t* event = (wifi_event_sta_wps_er_pin_t*) event_data;
                //printf("WPS_PIN = " PINSTR "\n", PIN2STR(event->pin_code));
                break;
            }
        default:
            break;
    }
    
}
// HTTP Error (404) Handler - Redirects all requests to the root page
esp_err_t esp32_c3_WIFI::http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    // Set status
    httpd_resp_set_status(req, "302 Temporary Redirect");
    // Redirect to the "/" root directory
    httpd_resp_set_hdr(req, "Location", "/");
    // iOS requires content in the response to detect a captive portal, simply redirecting is not sufficient.
    httpd_resp_send(req, "Redirect to the captive portal", HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

esp_err_t esp32_c3_WIFI::http_get_handler(httpd_req_t *req) {
    char*  buf;
    size_t buf_len;
    esp32_c3_WIFI* myself = (esp32_c3_WIFI*)req->user_ctx;
   /*
    
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        buf = (char*)malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Host: %s", buf);
        }
        free(buf);
    }

   */

    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = (char*)malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            PocuterWIFI::wifiCredentials cred;
            if (httpd_query_key_value(buf, "ssid", (char*)cred.ssid, 32) == ESP_OK) {
                if (httpd_query_key_value(buf, "passkey", (char*)cred.password, 64) == ESP_OK) {
                    httpd_resp_set_type(req, "text/html");
                    httpd_resp_send_chunk(req, myself->m_head, HTTPD_RESP_USE_STRLEN);
                    httpd_resp_send_chunk(req, myself->m_webPageConnecting, HTTPD_RESP_USE_STRLEN);
                    httpd_resp_send_chunk(req, myself->m_foot, HTTPD_RESP_USE_STRLEN);
                    httpd_resp_send_chunk(req, NULL, 0);
                    
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                    
                    myself->wifiDeInit();
                    myself->wifiInit();
                    wifi_config_t wifi_config = {
                        .sta = {
                            .pmf_cfg = {
                                .capable = true,
                                .required = false
                            },
                        },
                    };
                    strncpy((char*)wifi_config.sta.ssid, (char*)cred.ssid, sizeof(wifi_config.sta.ssid));
                    strncpy((char*)wifi_config.sta.password, (char*)cred.password, sizeof(wifi_config.sta.password));
                    if (myself->saveConfigOnSDCard(&wifi_config) != WIFIERROR_OK) {
                        printf("Could not save config on SD Card!\n");
                    }
                    esp_wifi_set_mode(WIFI_MODE_STA);
                    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
                    esp_wifi_connect();
                    free(buf);
                    
                    esp_restart();
                    return ESP_OK;
                }
            }

        }
        
    }
    
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send_chunk(req, myself->m_head, HTTPD_RESP_USE_STRLEN);
    httpd_resp_send_chunk(req, myself->m_webPageForm, HTTPD_RESP_USE_STRLEN);
    httpd_resp_send_chunk(req, myself->m_foot, HTTPD_RESP_USE_STRLEN);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
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
    wifiDeInit();
    wifiInit();
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
    if (saveConfigOnSDCard(&wifi_config) != WIFIERROR_OK) {
        printf("Could not save config on SD Card!\n");
    }
    if (esp_wifi_set_config(WIFI_IF_STA, &wifi_config) != ESP_OK) return WIFIERROR_INIT_FAILED;
    ret = esp_wifi_start();
    if (ret != ESP_OK) return WIFIERROR_INIT_FAILED;
    ret = esp_wifi_connect();
    if (ret != ESP_OK) return WIFIERROR_INIT_FAILED;
    return WIFIERROR_OK;
}
PocuterWIFI::WIFIERROR esp32_c3_WIFI::connect() {
    wifiDeInit();
    wifiInit();
    esp_err_t ret = esp_wifi_set_mode(WIFI_MODE_STA);
    if (ret != ESP_OK) return WIFIERROR_COULD_NOT_SET_WIFI_MODE;
    wifi_config_t wifi_config;
    if (saveConfigOnSDCard(&wifi_config) != WIFIERROR_OK) {
        printf("Could not save config on SD Card!\n");
    } else {
        esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    }
    
    
    ret = esp_wifi_start();
    if (ret != ESP_OK) return WIFIERROR_INIT_FAILED;
    ret = esp_wifi_connect();
    if (ret != ESP_OK) return WIFIERROR_INIT_FAILED;
    
    return WIFIERROR_OK;
}
PocuterWIFI::WIFIERROR esp32_c3_WIFI::startAccessPoint() {
    if (m_state == WIFI_WAITING_AP) return WIFIERROR_OK; //already started
    wifiDeInit();
    
    m_sta_netif = esp_netif_create_default_wifi_ap();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
//    esp_netif_dhcps_option();
    
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &esp32_c3_WIFI::wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid_len = strlen(ESP_WIFI_SSID),
            .channel = 1,
            .authmode = WIFI_AUTH_OPEN,
            .max_connection = 1,
            .beacon_interval = 100,
            .pairwise_cipher = WIFI_CIPHER_TYPE_TKIP,
            .ftm_responder = false
        }
        
    };
    strncpy((char*)wifi_config.ap.ssid, ESP_WIFI_SSID, 32);
    wifi_config.ap.password[0] = 0;
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    
    start_webserver();
    m_state = WIFI_WAITING_AP;
    return WIFIERROR_OK;
}

PocuterWIFI::WIFIERROR esp32_c3_WIFI::startWPS() {
    if (m_state == WIFI_WAITING_WPS) return WIFIERROR_OK; //already started
    wifiDeInit();
    
    esp_err_t ret;
    m_s_ap_creds_num = 0;
    m_RetryNum = 0;
    m_wpsConfig.wps_type = WPS_TYPE_PBC;
    m_state = WIFI_WAITING_WPS;
    
    strncpy(m_wpsConfig.factory_info.device_name, "POCUTER", 33);
    strncpy(m_wpsConfig.factory_info.manufacturer, "POCUTER GMBH", 33);
    strncpy(m_wpsConfig.factory_info.model_name, "POCUTER 1", 33);
    strncpy(m_wpsConfig.factory_info.model_number, "ONE", 33);
    
    wifiInit();
    
    
    ret = esp_wifi_set_mode(WIFI_MODE_STA);
    if (ret != ESP_OK) return WIFIERROR_COULD_NOT_SET_WIFI_MODE;
    
    ret = esp_wifi_start();
    if (ret != ESP_OK) return WIFIERROR_INIT_FAILED;
    
    esp_wifi_wps_enable(&m_wpsConfig);
    esp_wifi_wps_start(0);
    
    return WIFIERROR_OK;
}
void esp32_c3_WIFI::stop_webserver() {
    httpd_stop(m_httpServer);
    
    
}
void esp32_c3_WIFI::start_webserver() {
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    uri_get.user_ctx = this;
    
    /* Start the httpd server */
    if (httpd_start(&m_httpServer, &config) == ESP_OK) {
        /* Register URI handlers */
        httpd_register_uri_handler(m_httpServer, &uri_get);
        httpd_register_err_handler(m_httpServer, HTTPD_404_NOT_FOUND, http_404_error_handler);
        //httpd_register_uri_handler(server, &uri_post);
    }
    if (m_dns == NULL) m_dns = new esp32_c3_CaptivePortalDNS();
    
    /* If server failed to start, handle will be NULL */
    //return server;
}
PocuterWIFI::WIFIERROR esp32_c3_WIFI::scanAPs(apInfo* ai, uint16_t* size, uint16_t* totalAPs) {
    wifiInit();
    *totalAPs = 0;
    wifi_ap_record_t ap_info[*size];
    memset(ap_info, 0, sizeof(ap_info));
    esp_wifi_scan_start(NULL, true);
    esp_wifi_scan_get_ap_records(size, ap_info);
    esp_wifi_scan_get_ap_num(totalAPs);
    for (int i = 0; (i < *size) && (i < *totalAPs); i++) {
        ai[i].authMode = (AUTH_MODE)ap_info[i].authmode;
        memcpy(ai[i].bssid, ap_info[i].bssid, 6);
        memcpy(ai[i].ssid, ap_info[i].ssid, 33);
        ai[i].channel = ap_info[i].primary;
        ai[i].signalStrength = ap_info[i].rssi;
    }
    return WIFIERROR_OK;
}
esp32_c3_WIFI::~esp32_c3_WIFI() {
    esp_wifi_stop();
    esp_wifi_deinit();
}

#pragma GCC diagnostic pop 


#endif
