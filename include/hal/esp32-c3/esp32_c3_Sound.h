#ifndef ESP32_C3_SOUND_H
#define ESP32_C3_SOUND_H

#include "include/hal/PocuterSound.h"
#include "include/hal/esp32-c3/pwmSound/pwm_audio.h"

namespace PocuterLib{
    namespace HAL {
        class esp32_c3_Sound  : public PocuterSound {
        public:
            esp32_c3_Sound();
            
            virtual ~esp32_c3_Sound();
            
            SOUNDERROR init(SAMPLE_RATE sr, SOUND_CHANNELS c, SOUND_BIT_RATE b);
            SOUNDERROR start();
            SOUNDERROR stop();
            SOUNDERROR write(uint8_t *inbuf, size_t inbuf_len, size_t *bytes_written);
            SOUNDERROR setVolume(int8_t volume);
            SOUNDERROR getVolume(int8_t *volume);
        
        private:
            
        };
    }
}

#endif /* ESP32_C3_SOUND_H */

