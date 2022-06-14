#ifndef POCUTERWIFI_H
#define POCUTERWIFI_H
#include <stdint.h>

class PocuterWIFI {
    public:
        struct wifiCredentials {
            uint8_t ssid[32];      /**< SSID of target AP. */
            uint8_t password[64];  /**< Password of target AP. */
        };
        struct ipInfo {
            uint32_t ipV4;
            uint32_t netmask;
            uint32_t gw;
        };
       
        enum AUTH_MODE {
            WIFIAUTH_OPEN = 0,         /**< authenticate mode : open */
            WIFIAUTH_WEP,              /**< authenticate mode : WEP */
            WIFIAUTH_WPA_PSK,          /**< authenticate mode : WPA_PSK */
            WIFIAUTH_WPA2_PSK,         /**< authenticate mode : WPA2_PSK */
            WIFIAUTH_WPA_WPA2_PSK,     /**< authenticate mode : WPA_WPA2_PSK */
            WIFIAUTH_WPA2_ENTERPRISE,  /**< authenticate mode : WPA2_ENTERPRISE */
            WIFIAUTH_WPA3_PSK,         /**< authenticate mode : WPA3_PSK */
            WIFIAUTH_WPA2_WPA3_PSK,    /**< authenticate mode : WPA2_WPA3_PSK */
            WIFIAUTH_WAPI_PSK,         /**< authenticate mode : WAPI_PSK */
            WIFIAUTH_MAX
        };

        struct apInfo {
            uint8_t ssid[33];
            uint8_t bssid[6];
            uint8_t channel; 
            int8_t  signalStrength;
            AUTH_MODE authMode;
        };
        
       enum WIFIERROR {
           WIFIERROR_OK = 0,
           WIFIERROR_INIT_FAILED,
           WIFIERROR_COULD_NOT_SET_WIFI_MODE,
           WIFIERROR_NO_CREDENTIALS,
           
           
           
           
           
           WIFIERROR_UNKNOWN
           
          
       };
       enum WIFIEVENT {
           WIFIEVENT_DISCONNECTED,
           WIFIEVENT_CONNECTED,
           WIFIEVENT_GOT_IP,
           WIFIEVENT_WPS_TIMEOUT,
           WIFIEVENT_WPS_FAILED,
           
       };
       
       enum WIFI_STATE {
            WIFI_STATE_INIT_FAILED,     
            WIFI_STATE_DISCONNECTED,
            WIFI_STATE_CONNECTED,
            WIFI_WAITING_WPS,
            WIFI_WAITING_AP     
        };
            
            
       
       typedef void (wifiEventHandler)(WIFIEVENT, void* data, void* userData);
       
       virtual WIFI_STATE getState() = 0;
       virtual void registerEventHandler(wifiEventHandler*, void*) = 0;
       virtual WIFIERROR startWPS() = 0;
       virtual WIFIERROR startAccessPoint() = 0;
       virtual WIFIERROR getCredentials(wifiCredentials*) = 0;
       virtual WIFIERROR connect() = 0;
       virtual WIFIERROR connect(const wifiCredentials*) = 0;
       virtual WIFIERROR scanAPs(apInfo*, uint16_t* size, uint16_t* totalAPs) = 0;
    private:

};



#endif /* POCUTERWIFI_H */

