
#ifndef ESP32_C3_CAPTIVEPORTALDNS_H
#define ESP32_C3_CAPTIVEPORTALDNS_H

#include "freertos/task.h"

class esp32_c3_CaptivePortalDNS {
public:
    esp32_c3_CaptivePortalDNS();
    
    virtual ~esp32_c3_CaptivePortalDNS();
private:
    static void captdnsTask(void *pvParameters);
    static int s_sockFd;
    static void  captdnsRecv(struct sockaddr_in *premote_addr, char *pusrdata, unsigned short length);
    TaskHandle_t m_taskHandle;
    typedef struct __attribute__ ((packed)) {
            uint16_t id;
            uint8_t flags;
            uint8_t rcode;
            uint16_t qdcount;
            uint16_t ancount;
            uint16_t nscount;
            uint16_t arcount;
    } DnsHeader;


    typedef struct __attribute__ ((packed)) {
            uint8_t len;
            uint8_t data;
    } DnsLabel;


    typedef struct __attribute__ ((packed)) {
            //before: label
            uint16_t type;
            uint16_t dclass;
    } DnsQuestionFooter;


    typedef struct __attribute__ ((packed)) {
            //before: label
            uint16_t type;
            uint16_t dclass;
            uint32_t ttl;
            uint16_t rdlength;
            //after: rdata
    } DnsResourceFooter;

    typedef struct __attribute__ ((packed)) {
            uint16_t prio;
            uint16_t weight;
    } DnsUriHdr;
    
    


};

#endif /* ESP32_C3_CAPTIVEPORTALDNS_H */

