#ifndef POCUTERWIFI_H
#define POCUTERWIFI_H
#include <stdint.h>

/**
* @brief Pocuter WIFI configurations
*  
*/

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
           WIFIERROR_WPS_TIMEOUT,
           
           
           
           
           WIFIERROR_UNKNOWN
           
          
       };
       enum WIFIEVENT {
           WIFIEVENT_DISCONNECTED,
           WIFIEVENT_CONNECTED,
           WIFIEVENT_TRY_CONNECTING,
           WIFIEVENT_GOT_IP,
           WIFIEVENT_WPS_TIMEOUT,
           WIFIEVENT_WPS_FAILED,
           
       };
       
       enum WIFI_STATE {
            WIFI_STATE_INIT_FAILED,     
            WIFI_STATE_DISCONNECTED,
            WIFI_STATE_TRY_CONNECTING,
            WIFI_STATE_CONNECTED,
            WIFI_WAITING_WPS,
            WIFI_WAITING_AP     
        };
            
            
       /**
        * @brief typedef of the event handler
        * 
        * @param WIFIEVENT the type of event
        * @param data additionally event data
        * @param userData a pointer to userdata, set by registerEventHandler
        */
       typedef void (wifiEventHandler)(WIFIEVENT, void* data, void* userData);
       
       /**
        * @brief get the state of the WIFI
        * 
        * @return WIFI_STATE
        */
       virtual WIFI_STATE getState() = 0;
       
       /**
        * @brief registers an event handler
        * 
        * @param h a pointer to the callback function
        * @param u a pointer to user data sent to the event handler on each event
        * 
        */
       virtual void registerEventHandler(wifiEventHandler* h, void* u) = 0;
       
       /**
        * @brief starts WPS request.
        * 
        * @note the function will block until a timeout or the WPS was successful
        * 
        * @return 
        *           WIFIERROR_WPS_TIMEOUT it timed out  
        *           WIFIERROR_OK a new connection was established
        */
       virtual WIFIERROR startWPS() = 0;
       
       
       
       /**
        * @brief starts an access point to set the WIFI credentials
        * 
        * @note the access point will be called "Pocuter". The function will not block. The Pocter will restart when the credentials were set. 
        * 
        * @return 
        *           WIFIERROR_OK the access point is available
        */
       virtual WIFIERROR startAccessPoint() = 0;
       
       /**
        * @brief get the current credentials
        * 
        * @param c (out) a pointer to a wifiCredentials struct
        * 
        * @return 
        *           WIFIERROR_OK the access point is available
        */
       virtual WIFIERROR getCredentials(wifiCredentials* c) = 0;
       
       /**
        * @brief connect to wifi with saved credentials
        * 
        * @note The function will not block. You can see the current state by polling getState or setting the eventhandler.
        * 
        * @return 
        *           WIFIERROR_OK the access point is available
        *           WIFIERROR_INIT_FAILED could not init WIFI
        */
       virtual WIFIERROR connect() = 0;
       
       /**
        * @brief connect to wifi with given credentials
        * 
        * @note The function will not block. You can see the current state by polling getState or setting the eventhandler.
        * 
        * @param c a pointer to a wifiCredentials struct with credentials
        * 
        * @return 
        *           WIFIERROR_OK the access point is available
        *           WIFIERROR_INIT_FAILED could not init WIFI
        */
       virtual WIFIERROR connect(const wifiCredentials* c) = 0;
       
       /**
        * @brief scan for access points
        * 
        * @note this function will block. You have to allocate the memory of the apInfo array and set the size in the size parameter. 
        * 
        * @param i an array of apInfo's already allocated
        * @param size (in and out) set this to the cont of the allocated apInfo elements and it will be set to the actually saved access point count. 
        * @param totalAPs (out) the total number of access points found
        * 
        * @return 
        *           WIFIERROR_OK the access point is available
        *           WIFIERROR_INIT_FAILED could not init WIFI
        */
       virtual WIFIERROR scanAPs(apInfo* i, uint16_t* size, uint16_t* totalAPs) = 0;
       
       /**
        * @brief get ipInfo
        * 
        * @note you will only get an ipInfo if you are connected. Otherwise you will get NULL.
        * 
        * @return 
        *           ipInfo the ipInfo struct
        *           NULL if you are not connected
        */
       virtual const ipInfo* getIpInfo() = 0;
       
    private:

};



#endif /* POCUTERWIFI_H */

