#include "include/PocuterLibConfig.h"

#ifndef POCUTER_DISABLE_SERVER   
#include "include/hal/PocuterServer.h"
#include "include/tiny-json/tiny-json.h"
#include "include/hal/PocuterHMAC.h"


#include <inttypes.h>

 // Let's Encrypt root ca
    const char* PocuterServer::letsEncryptRootCA = "------BEGIN CERTIFICATE-----\n\
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n\
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n\
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n\
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n\
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n\
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n\
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n\
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n\
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n\
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n\
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n\
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n\
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n\
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n\
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n\
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n\
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n\
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n\
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n\
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n\
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n\
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n\
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n\
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n\
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n\
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n\
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n\
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n\
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n\
-----END CERTIFICATE-----\n\n";

    
    // GlobalSign Root CA
    const char* PocuterServer::globalSignRootCA = "-----BEGIN CERTIFICATE-----\n\
MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG\n\
A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\n\
b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw\n\
MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\n\
YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT\n\
aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ\n\
jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp\n\
xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp\n\
1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG\n\
snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ\n\
U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8\n\
9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E\n\
BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B\n\
AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz\n\
yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE\n\
38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP\n\
AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad\n\
DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME\n\
HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==\n\
-----END CERTIFICATE-----\n\n";

    
    

PocuterServer::PocuterServer(PocuterHMAC* pHMAC, PocuterOTA* pOTA, PocuterHTTP* pHTTP) {
    m_HMAC = pHMAC;
    m_pOTA = pOTA;
    m_pHTTP = pHTTP;
}


PocuterServer::~PocuterServer() {
}
const uint8_t* PocuterServer::getServerRootCa() {
    return (uint8_t*)globalSignRootCA;
}
const uint8_t* PocuterServer::checkNewestAppVersion(uint64_t appId) {
    uint8_t* chipID = m_HMAC->getChipID();
    uint8_t major, minor, patch;
    PocuterOTA::OTAERROR otaErr = m_pOTA->getAppVersion(appId, &major, &minor, &patch);
    if (otaErr != PocuterOTA::OTAERROR_OK){ major = 0; minor = 0; patch = 0; }
    char buf[64];
    char* response = new char[1024];
    snprintf(buf, 64, "https://psus.live/%s/%" PRIu64 "/%d.%d.%d", (char*)chipID, appId, major, minor, patch);
    bool haveDownload = false;
    if (m_pHTTP->getResponse((uint8_t*)buf, (uint8_t*)response, 1024, 3000, (const uint8_t*)globalSignRootCA) == PocuterHTTP::HTTPERROR_OK) {
        json_t mem[32];
        json_t const* json = json_create( response, mem, sizeof mem / sizeof *mem );
        if ( json ) {
            json_t const* url = json_getProperty( json, "u" );
            if ( url  && JSON_TEXT == json_getType( url ) ){
                strncpy((char*)m_buffer, json_getValue(url), 64);
                haveDownload = true;
            }
        }
    }
    delete[] response;
    
    if (! haveDownload) return NULL;
    return m_buffer;
}

const uint8_t* PocuterServer::checkForAppInstallRequest(uint64_t& appId) {
    uint8_t* chipID = m_HMAC->getChipID();
    appId = 0;        
    
    char buf[64];
    char* response = new char[1024];
    snprintf(buf, 64, "https://si.psus.live/%s", (char*)chipID);
    bool haveDownload = false;
    if (m_pHTTP->getResponse((uint8_t*)buf, (uint8_t*)response, 1024, 3000, (const uint8_t*)globalSignRootCA) == PocuterHTTP::HTTPERROR_OK) {
        json_t mem[32];
        json_t const* json = json_create( response, mem, sizeof mem / sizeof *mem );
        if ( json ) {
            json_t const* url = json_getProperty( json, "u" );
            if ( url  && JSON_TEXT == json_getType( url ) ){
                strncpy((char*)m_buffer, json_getValue(url), 64);
                json_t const* jappId = json_getProperty( json, "i" );
                if ( jappId  && JSON_INTEGER == json_getType( jappId ) ){
                    appId =  json_getInteger(jappId);
                    haveDownload = true;
                }
            }
            
        }
    }
    delete[] response;
    
    if (! haveDownload) return NULL;
    return m_buffer;
}
bool PocuterServer::appInstalledSuccessfully(uint64_t appId) {
    uint8_t* chipID = m_HMAC->getChipID();
    bool ok = false;
    char buf[64];
    char* response = new char[1024];
    snprintf(buf, 64, "https://si.psus.live/%s/%" PRIu64 , (char*)chipID, appId);
    if (m_pHTTP->getResponse((uint8_t*)buf, (uint8_t*)response, 1024, 3000, (const uint8_t*)globalSignRootCA) == PocuterHTTP::HTTPERROR_OK) {
        json_t mem[32];
        json_t const* json = json_create( response, mem, sizeof mem / sizeof *mem );
        if ( json ) {
            json_t const* state = json_getProperty( json, "state" );
            if ( state  && JSON_TEXT == json_getType( state ) ){
                if (0 == strncmp(json_getValue(state), "ok", 2)) ok = true;
            }
            
        }
    }
    delete[] response;
    return ok;
    
    
}

bool PocuterServer::getAppStoreList(AppStoreEntry *list, int *count, int offset) {
    uint8_t* chipID = m_HMAC->getChipID();
    char url[64];
    char* response = new char[1024];
    snprintf(url, 64, "https://dias.psus.live/%s/list/default/%d-%d", (char*)chipID, *count, offset);
    
    if (m_pHTTP->getResponse((uint8_t*)url, (uint8_t*)response, 1024, 3000, (const uint8_t*)globalSignRootCA) != PocuterHTTP::HTTPERROR_OK) {
        delete[] response;
        *count = 0;
        return false;
    }
    json_t mem[64];
    json_t const* json = json_create( response, mem, sizeof mem / sizeof *mem );
    if (!json) {
        delete[] response;
        *count = 0;
        return false;
    }
    
    int appCount = 0;
    const json_t *child = json_getChild(json);
    
    while (child != NULL && appCount < *count) {
        AppStoreEntry *entry = &list[appCount];
        
        // entries should be empty
        strcpy(entry->name, "");
        strcpy(entry->author, "");
        strcpy(entry->version, "");
        
        const json_t *id = json_getProperty(child, "id");
        const json_t *name = json_getProperty(child, "n");
        const json_t *author = json_getProperty(child, "a");
        const json_t *version = json_getProperty(child, "v");
        
        if (id) {
            entry->id = json_getInteger(id);
            
            if (name)
                strncpy(entry->name, json_getValue(name), 64);
            
            if (author)
                strncpy(entry->author, json_getValue(author), 64);
            
            if (version)
                strncpy(entry->version, json_getValue(version), 16);
        }
        
        appCount += 1;
        child = json_getSibling(child);
    }
    delete[] response;
    
    *count = appCount;
    return true;
}

#endif
