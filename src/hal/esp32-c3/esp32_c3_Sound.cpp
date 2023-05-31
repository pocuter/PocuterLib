#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_SOUND

#include "include/hal/esp32-c3/esp32_c3_Sound.h"
#include "include/hal/PocuterDeviceType.h"

using namespace PocuterLib::HAL;
esp32_c3_Sound::esp32_c3_Sound() {
    
   
}


esp32_c3_Sound::~esp32_c3_Sound() {
}

PocuterSound::SOUNDERROR esp32_c3_Sound::init(SAMPLE_RATE sr, SOUND_CHANNELS c, SOUND_BIT_RATE b) {
    pwm_audio_config_t pac;
    pac.duty_resolution    = LEDC_TIMER_10_BIT;
    if (PocuterDeviceType::deviceType == PocuterDeviceType::DEVICE_TYPE_POCUTER_1) {
        pac.gpio_num_left      = GPIO_NUM_3;
    } else {
        pac.gpio_num_left      = GPIO_NUM_6;
    }
    
    pac.ledc_channel_left  = LEDC_CHANNEL_0;
    pac.gpio_num_right     = -1;
    pac.ledc_channel_right = LEDC_CHANNEL_1;
    pac.ledc_timer_sel     = LEDC_TIMER_0;
    pac.tg_num             = TIMER_GROUP_0;
    pac.timer_num          = TIMER_0;
    pac.ringbuf_len        = 1024*8;

    if (ESP_OK == pwm_audio_init(&pac)) {
        if (ESP_OK == pwm_audio_set_param(sr, (ledc_timer_bit_t)b, c)) {
            return SOUNDERROR_OK;
        }
    }
        
   return SOUNDERROR_FAILED;
}

PocuterSound::SOUNDERROR esp32_c3_Sound::start() {
    if (ESP_OK == pwm_audio_start()) return SOUNDERROR_OK;
    return SOUNDERROR_FAILED;
}
PocuterSound::SOUNDERROR esp32_c3_Sound::stop() {
    if (ESP_OK == pwm_audio_stop()) return SOUNDERROR_OK;
    return SOUNDERROR_FAILED;
}
PocuterSound::SOUNDERROR esp32_c3_Sound::write(uint8_t *inbuf, size_t inbuf_len, size_t *bytes_written) {
    if (ESP_OK == pwm_audio_write(inbuf, inbuf_len, bytes_written, 100 / portTICK_PERIOD_MS)) return SOUNDERROR_OK;
    return SOUNDERROR_FAILED;
}
PocuterSound::SOUNDERROR esp32_c3_Sound::setVolume(int8_t volume) {
    if (ESP_OK == pwm_audio_set_volume(volume)) return SOUNDERROR_OK;
    return SOUNDERROR_FAILED;
}
PocuterSound::SOUNDERROR esp32_c3_Sound::getVolume(int8_t *volume) {
    if (ESP_OK == pwm_audio_get_volume(volume)) return SOUNDERROR_OK;
    return SOUNDERROR_FAILED;
}

#endif
