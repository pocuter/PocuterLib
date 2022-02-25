
#include "include/hal/esp32-c3/esp32_c3_ADC.h"

using namespace PocuterLib::HAL;
#define TIMES              256
esp32_c3_ADC::esp32_c3_ADC() {
    
}


esp32_c3_ADC::~esp32_c3_ADC() {
}

int esp32_c3_ADC::singleRead(adc1_channel_t channel) {
    adc1_config_width((adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(channel, ADC_ATTEN_DB_0);
    return adc1_get_raw(channel);
}
