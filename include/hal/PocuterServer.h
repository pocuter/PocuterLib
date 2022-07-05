
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
    static const char* letsEncryptRootCA;
private:
    PocuterHMAC* m_HMAC;
    PocuterOTA* m_pOTA;
    PocuterHTTP* m_pHTTP;
    
    uint8_t m_buffer[64];
};

#endif /* POCUTERSERVER_H */

