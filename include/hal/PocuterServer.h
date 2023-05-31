
#ifndef POCUTERSERVER_H
#define POCUTERSERVER_H
#include "include/hal/PocuterHMAC.h"
#include "include/hal/PocuterOTA.h"
#include "include/hal/PocuterHTTP.h"

class PocuterServer {
public:
    PocuterServer(PocuterHMAC*, PocuterOTA*, PocuterHTTP*);
    virtual ~PocuterServer();
    
    const uint8_t* checkNewestAppVersion(uint64_t appId);
    const uint8_t* getServerRootCa();
    const uint8_t* checkForAppInstallRequest(uint64_t& appId);
    bool appInstalledSuccessfully(uint64_t appId);
    
    static const char* letsEncryptRootCA;
    static const char* globalSignRootCA;
    
    struct AppStoreEntry {
        uint64_t id;
        char name[64];
        char author[64];
        char version[16];
    };
    
    bool getAppStoreList(AppStoreEntry *list, int *count, int offset);
    
private:
    PocuterHMAC* m_HMAC;
    PocuterOTA* m_pOTA;
    PocuterHTTP* m_pHTTP;
    
    uint8_t m_buffer[64];
};

#endif /* POCUTERSERVER_H */

