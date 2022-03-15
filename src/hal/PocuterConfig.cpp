
#include "include/hal/PocuterConfig.h"
#include "include/minIni/minIni.h"
#include "include/hal/PocuterSDCard.h"
#include <sys/stat.h>
#include "mbedtls/base64.h"
#include "mbedtls/aes.h"
#include "include/hal/PocuterHMAC.h"
#include "include/hal/esp32-c3/esp32_c3_hmac.h"
#include <string.h>
#include <inttypes.h>
#define CONFIG_PATH "/sd/config"
PocuterConfig::PocuterConfig(const uint8_t* appName) {
   m_configFile =  (uint8_t *)malloc(128);
   mkdir(CONFIG_PATH, S_IRWXU);
   snprintf((char*)m_configFile, 128, "%s/%s.ini", CONFIG_PATH, appName);
   m_readony = false;
}
PocuterConfig::PocuterConfig(uint64_t appID) {
    m_configFile =  (uint8_t *)malloc(128);
    snprintf((char*)m_configFile, 128, "%s/%" PRIu64 "/esp32c3.app", CONFIG_PATH, appID);
    m_readony = true;
}

PocuterConfig::~PocuterConfig() {
    free(m_configFile);
}

bool PocuterConfig::get(const uint8_t* section, const uint8_t* name, uint8_t* result, size_t maxLength) {
     long  n = ini_gets((const TCHAR*)section, (const TCHAR*)name, "", (TCHAR*)result, maxLength, (const TCHAR*)m_configFile);
     return (n>0);
}
bool PocuterConfig::set(const uint8_t* section, const uint8_t* name, const uint8_t* value) {
    if (m_readony) return false;
    long  n = ini_puts((const TCHAR*)section, (const TCHAR*)name, (const TCHAR*)value, (const TCHAR*)m_configFile);
    return (n>0);
}
bool PocuterConfig::del(const uint8_t* section, const uint8_t* name) {
    ini_puts((const TCHAR*)section, (const TCHAR*)name, NULL, (const TCHAR*)m_configFile);
    return true;
}
#ifndef POCUTER_NO_CONFIG_ENCRYPTION
bool PocuterConfig::getEncrypted(const uint8_t* section, const uint8_t* name, uint8_t* result, size_t maxLength) {
    if (maxLength < 64) return false;
    uint8_t input[64];
    if (! getBinary(section, name, input, 64)){ return false;}
    PocuterHMAC* hmac = new PocuterLib::HAL::esp32_c3_hmac();
    mbedtls_aes_context aes;
    unsigned char iv[] = {0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0xAA, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    uint8_t reply[32];
    
   
    uint8_t replyLength = 32;
    hmac->calculateAESKey(name, strlen((const char*)name), reply, replyLength);
    delete((PocuterLib::HAL::esp32_c3_hmac*)hmac);
    if (replyLength != 32) return false;
    
    mbedtls_aes_setkey_enc( &aes, reply, 32*8 );
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, 64, iv, input, result);
    
    return true;
}
bool PocuterConfig::setEncrypted(const uint8_t* section, const uint8_t* name, const uint8_t* value) {
    if (m_readony) return false;
    if (strlen((const char*)value) > 64) return false;
    PocuterHMAC* hmac = new PocuterLib::HAL::esp32_c3_hmac();
    mbedtls_aes_context aes;
    unsigned char iv[] = {0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0xAA, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    uint8_t reply[32];
    uint8_t input[64];
    uint8_t output[64];
    memset(input, 0, 64);
    memcpy(input, value, 64);
    
    uint8_t replyLength = 32;
    hmac->calculateAESKey(name, strlen((const char*)name), reply, replyLength);
    delete((PocuterLib::HAL::esp32_c3_hmac*)hmac);
    if (replyLength != 32) return false;
    
    mbedtls_aes_setkey_enc( &aes, reply, 32*8 );
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, 64, iv, input, output);
    return setBinary(section, name, output, 64);
    
}
#endif
uint32_t PocuterConfig::get(const uint8_t* section, const uint8_t* name) {
    return ini_getl((const TCHAR*)section, (const TCHAR*)name, 0, (const TCHAR*)m_configFile);
}

bool PocuterConfig::set(const uint8_t* section, const uint8_t* name, uint32_t value) {
    if (m_readony) return false;
    return ini_putl((const TCHAR*)section, (const TCHAR*)name, value, (const TCHAR*)m_configFile);
}

bool PocuterConfig::getBinary(const uint8_t* section, const uint8_t* name, void* result, size_t maxLength) {
    uint8_t input[128];
    if (get(section, name, input, 128)) {
        size_t outlen;
        mbedtls_base64_decode((unsigned char *)result, maxLength, &outlen, (unsigned char *)input, strlen((char *)input));
        return (outlen > 0);
    }
    return false;
}
bool PocuterConfig::setBinary(const uint8_t* section, const uint8_t* name, const void* value, size_t valueLength) {
    if (m_readony) return false;
    uint8_t output[128];
    size_t outlen;
    mbedtls_base64_encode((unsigned char *)output, 128, &outlen, (unsigned char *)value, valueLength);
    return set(section, name, output);
}

