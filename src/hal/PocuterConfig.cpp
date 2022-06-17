
#include "include/hal/PocuterConfig.h"
#include "include/minIni/minIni.h"
#include "include/hal/PocuterSDCard.h"
#include <sys/stat.h>
#include "mbedtls/base64.h"
#include "mbedtls/aes.h"
#include "include/hal/PocuterHMAC.h"
#include "include/hal/esp32-c3/esp32_c3_hmac.h"
#include "include/hal/esp32-c3/esp32_c3_OTA.h"
#include <string.h>
#include <inttypes.h>
#define CONFIG_PATH "/sd/config"

PocuterConfig::PocuterConfig(const uint8_t* configName, uint64_t* pappID) {
   uint64_t appID = 1;
   if (pappID != NULL) {
       appID = *pappID;
   } else {
       appID = PocuterLib::HAL::esp32_c3_OTA::getCurrentAppID();
   }
   
   m_configFile =  (uint8_t *)malloc(128);
   mkdir(CONFIG_PATH, S_IRWXU);
   snprintf((char*)m_configFile, 128, "%s/%" PRIu64, CONFIG_PATH, appID);
   mkdir((char*)m_configFile, S_IRWXU);
   snprintf((char*)m_configFile, 128, "%s/%" PRIu64 "/%s.ini", CONFIG_PATH, appID, configName);
   m_readony = false;
}
PocuterConfig::PocuterConfig(uint64_t appID) {
    m_configFile =  (uint8_t *)malloc(128);
    snprintf((char*)m_configFile, 128, "/sd/apps/%" PRIu64 "/esp32c3.app", appID);
    m_readony = true;
}

PocuterConfig::~PocuterConfig() {
    free(m_configFile);
}

bool PocuterConfig::get(const uint8_t* section, const uint8_t* name, uint8_t* result, size_t maxLength, bool decrypt) {
     if (decrypt) return getEncrypted(section, name, result, maxLength);
     long  n = ini_gets((const TCHAR*)section, (const TCHAR*)name, "", (TCHAR*)result, maxLength, (const TCHAR*)m_configFile);
     return (n>0);
}
bool PocuterConfig::set(const uint8_t* section, const uint8_t* name, const uint8_t* value, bool encrypt) {
    if (m_readony) return false;
    if (encrypt) return setEncrypted(section, name, value);
    long  n = ini_puts((const TCHAR*)section, (const TCHAR*)name, (const TCHAR*)value, (const TCHAR*)m_configFile);
    return (n>0);
}
bool PocuterConfig::del(const uint8_t* section, const uint8_t* name) {
    ini_puts((const TCHAR*)section, (const TCHAR*)name, NULL, (const TCHAR*)m_configFile);
    return true;
}

#define ENC_BLOCKSIZE 16
bool PocuterConfig::getEncrypted(const uint8_t* section, const uint8_t* name, uint8_t* result, size_t maxLength) {
    size_t needSize = ENC_BLOCKSIZE;
    if (maxLength > needSize) needSize = maxLength + (ENC_BLOCKSIZE - (maxLength % ENC_BLOCKSIZE));
    bool res = false;
    
    
    uint8_t* input = new uint8_t[needSize];
    uint8_t* output = new uint8_t[needSize];
    
    if (getBinary(section, name, input, needSize)){ 
        PocuterHMAC* hmac = new PocuterLib::HAL::esp32_c3_hmac();
        mbedtls_aes_context aes;
        unsigned char iv[] = {0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0xAA, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
        uint8_t AESKey[32];
        uint8_t AESKeyLength = 32;
        hmac->calculateAESKey(name, strlen((const char*)name), AESKey, AESKeyLength);
        delete((PocuterLib::HAL::esp32_c3_hmac*)hmac);
        if (AESKeyLength == 32){
            mbedtls_aes_setkey_enc( &aes, AESKey, 32*8 );
            if (! mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, needSize, iv, input, output)) {
                memcpy(result, output, maxLength);
                res =  true;
            }
        }
        
        
    }
    delete[] input;
    delete[] output;
    return res;
}
bool PocuterConfig::setEncrypted(const uint8_t* section, const uint8_t* name, const uint8_t* value) {
    if (m_readony) return false;
    PocuterHMAC* hmac = new PocuterLib::HAL::esp32_c3_hmac();
    bool res = false;
    if (hmac->isEncryptionKeySet()) {
        size_t length = strlen((const char*)value) + 1;
        
        if (length <= 366) {
            size_t needSize = ENC_BLOCKSIZE;
            if (length > needSize) needSize = length + (ENC_BLOCKSIZE - (length % ENC_BLOCKSIZE));
            printf("needSize setEnc length: %d\n", needSize);
            mbedtls_aes_context aes;
            unsigned char iv[] = {0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0xAA, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
            uint8_t AESKey[32];
            uint8_t* input = new uint8_t[needSize];
            uint8_t* output = new uint8_t[needSize];
            memset(input, 0, needSize);
            memcpy(input, value, length);

            uint8_t AESKeyLength = 32;
            hmac->calculateAESKey(name, strlen((const char*)name), AESKey, AESKeyLength);
            if (AESKeyLength == 32){
                mbedtls_aes_setkey_enc( &aes, AESKey, 32*8 );
                mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, needSize, iv, input, output);
                res = setBinary(section, name, output, needSize);

            }
            delete[] input;
            delete[] output;
        }
    }
    delete((PocuterLib::HAL::esp32_c3_hmac*)hmac);
    
    return res;
    
}

uint32_t PocuterConfig::get(const uint8_t* section, const uint8_t* name) {
    return ini_getl((const TCHAR*)section, (const TCHAR*)name, 0, (const TCHAR*)m_configFile);
}

bool PocuterConfig::set(const uint8_t* section, const uint8_t* name, uint32_t value) {
    if (m_readony) return false;
    return ini_putl((const TCHAR*)section, (const TCHAR*)name, value, (const TCHAR*)m_configFile);
}

bool PocuterConfig::getBinary(const uint8_t* section, const uint8_t* name, void* result, size_t maxLength, bool decrypt) {
    size_t needSize = 4*(maxLength/3) + 8;
    uint8_t* input = new uint8_t[needSize];
    bool res = false;
    if (! decrypt) {
        res = get(section, name, input, needSize);
    } else {
        res = getEncrypted(section, name, input, needSize);
    }
    
    if (res) {
        size_t outlen;
        mbedtls_base64_decode((unsigned char *)result, maxLength, &outlen, (unsigned char *)input, strlen((char *)input));
        res = (outlen > 0);
    }
    delete[] input;
    return res;
}
bool PocuterConfig::setBinary(const uint8_t* section, const uint8_t* name, const void* value, size_t valueLength, bool encrypt) {
    printf("SetBinary %d: %d\n", encrypt, valueLength);
    if (m_readony) return false;
    if (encrypt && valueLength > 261) return false;
    
    size_t needSize = 4*(valueLength/3) + 8;
    uint8_t* output = new uint8_t[needSize];
    size_t outlen;
    mbedtls_base64_encode((unsigned char *)output, needSize, &outlen, (unsigned char *)value, valueLength);
    bool ret;
    if (! encrypt) {
        ret = set(section, name, output);
    } else {
        ret = setEncrypted(section, name, output);
    }
    
    
    delete[] output;
    return ret;
}

