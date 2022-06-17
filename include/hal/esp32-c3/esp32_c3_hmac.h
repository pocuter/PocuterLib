#ifndef ESP32_C3_HMAC_H
#define ESP32_C3_HMAC_H
#include <stdint.h>
#include "include/hal/PocuterHMAC.h"

namespace PocuterLib {
    namespace HAL {
        class esp32_c3_hmac : public PocuterHMAC{
        public:
            esp32_c3_hmac();
            virtual ~esp32_c3_hmac();
            bool isEncryptionKeySet();
            bool setEfuseKey(const uint8_t* masterKey, uint8_t keyLength);
            uint8_t* getChipID();
            void calculateChallengeReply(const uint8_t* challenge, uint8_t challengeLength, uint8_t* reply, uint8_t& replyLength);
            void calculateAESKey(const uint8_t* challenge, uint8_t challengeLength, uint8_t* reply, uint8_t& replyLength);
            void calculateChallengeReplySW(const uint8_t* key, uint8_t keyLength, const uint8_t* challenge, uint8_t challengeLength, uint8_t* reply, uint8_t& replyLength);
        private:
            uint8_t* calculateIndividualEfuseKey(const uint8_t* masterKey, uint8_t keyLength);
            uint8_t efuseKey[32];
            uint8_t chipID[13];
        };
    }
}

#endif /* ESP32_C3_HMAC_H */

