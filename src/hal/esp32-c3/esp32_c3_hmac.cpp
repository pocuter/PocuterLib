#include "include/hal/esp32-c3/esp32_c3_hmac.h"
#include <esp_err.h>
#include <esp32c3/rom/efuse.h>
#include <mbedtls/sha256.h>
#include "mbedtls/md.h"
#include <esp_mac.h>
#include <soc/esp32c3/esp_hmac.h>
#include <string.h>


using namespace PocuterLib::HAL;

#define SECURE_KEY_EFUSE_BLOCK ETS_EFUSE_BLOCK_KEY5
#define SECURE_KEY_HMAC HMAC_KEY5

esp32_c3_hmac::esp32_c3_hmac() {
    
}
esp32_c3_hmac::~esp32_c3_hmac() {
    
}
uint8_t* esp32_c3_hmac::calculateIndividualEfuseKey(const uint8_t* masterKey, uint8_t keyLength) {
    uint8_t mac[6];
   
    if (esp_efuse_mac_get_default(mac) == ESP_OK) {
        uint8_t* buffer = (uint8_t*)malloc(keyLength + 6);
        memcpy(buffer, masterKey, keyLength);
        memcpy(buffer + keyLength, mac, 6);
        mbedtls_sha256(buffer, keyLength + 6, efuseKey, 0);
        return efuseKey;
    }
    return NULL;
}

bool esp32_c3_hmac::setEfuseKey(const uint8_t* masterKey, uint8_t keyLength) {
    uint8_t* key = calculateIndividualEfuseKey(masterKey, keyLength);
    if (key && ets_efuse_write_key(SECURE_KEY_EFUSE_BLOCK,
                    ETS_EFUSE_KEY_PURPOSE_HMAC_UP,
                    key, 32) == ESP_OK) {
        return true;
        
    }
    return false;
}
uint8_t* esp32_c3_hmac::getChipID() {
    uint8_t mac[6];
   
    if (esp_efuse_mac_get_default(mac) == ESP_OK) {
        snprintf((char*)chipID, 13, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], mac[6]);
        return chipID;
    }
    return NULL;
}
void esp32_c3_hmac::calculateChallengeReply(const uint8_t* challenge, uint8_t challengeLength, uint8_t* reply, uint8_t& replyLength) {
    if (replyLength < 65) {replyLength = 0; return;}
    uint8_t hmac[32];
    if (esp_hmac_calculate(HMAC_KEY5, challenge, challengeLength, hmac) == ESP_OK) {
        for (int i = 0; i < 32; i++) {
            snprintf((char*)(reply + (i*2)), 3, "%02x", hmac[i]);
        }
        reply[64] = 0;
        replyLength = 65;
    } else {
        reply[0] = 0;
        replyLength = 0;
    }
}

void esp32_c3_hmac::calculateAESKey(const uint8_t* challenge, uint8_t challengeLength, uint8_t* reply, uint8_t& replyLength) {
    if (replyLength < 32) {replyLength = 0; return;}
    if (esp_hmac_calculate(HMAC_KEY5, challenge, challengeLength, reply) == ESP_OK) {
        replyLength = 32;
    } else {
        reply[0] = 0;
        replyLength = 0;
    }
}


void esp32_c3_hmac::calculateChallengeReplySW(const uint8_t* masterKey, uint8_t keyLength, const uint8_t* challenge, uint8_t challengeLength, uint8_t* reply, uint8_t& replyLength) {
    if (replyLength < 65){replyLength = 0; return;}
    uint8_t hmac[32];
    uint8_t mac[6];
    reply[0] = 0;
    replyLength = 0;
    if (esp_efuse_mac_get_default(mac) == ESP_OK) {
        mbedtls_md_context_t ctx;
        mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
        uint8_t* k = calculateIndividualEfuseKey(masterKey, keyLength);
        if (k) {
            mbedtls_md_init(&ctx);
            mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
            mbedtls_md_hmac_starts(&ctx, (const unsigned char *) k, 32);
            mbedtls_md_hmac_update(&ctx, (const unsigned char *) challenge, challengeLength);
            mbedtls_md_hmac_finish(&ctx, hmac);
            mbedtls_md_free(&ctx);

            for (int i = 0; i < 32; i++) {
                snprintf((char*)(reply + (i*2)), 3, "%02x", hmac[i]);
            }
            reply[64] = 0;
            replyLength = 65;
        }
    }
}
