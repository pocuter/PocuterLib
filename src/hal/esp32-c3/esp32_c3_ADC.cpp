
#include "include/hal/esp32-c3/esp32_c3_ADC.h"
#include <string.h>

using namespace PocuterLib::HAL;
#define TIMES              256

esp32_c3_ADC* esp32_c3_ADC::m_pInstance = NULL;


esp32_c3_ADC::esp32_c3_ADC() {
   m_continuousReading = false;
   m_dataEventHandler = NULL;
   
    m_eventHandlerSemaphore = xSemaphoreCreateBinary();
    if (!m_eventHandlerSemaphore) abort();
    xSemaphoreGive(m_eventHandlerSemaphore);
    
}


esp32_c3_ADC::~esp32_c3_ADC() {
}

int esp32_c3_ADC::singleRead(adc1_channel_t channel) {
    adc1_config_width((adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(channel, ADC_ATTEN_DB_0);
    return adc1_get_raw(channel);
}
esp32_c3_ADC* esp32_c3_ADC::Instance() {
    if (!m_pInstance) {
        m_pInstance = new esp32_c3_ADC();
    }
    return m_pInstance;
}
bool esp32_c3_ADC::unregisterContinuousRead() {
    xSemaphoreTake(m_eventHandlerSemaphore, portMAX_DELAY);
    m_continuousReading = false;
    m_dataEventHandler = NULL;
    xSemaphoreGive(m_eventHandlerSemaphore);
    return true;
}
bool esp32_c3_ADC::registerContinuousRead(esp32_c3_ADC::adcEventHandlerDataEvailable* eventHandler, void* ud) {
    xSemaphoreTake(m_eventHandlerSemaphore, portMAX_DELAY);
    bool ok = false;
    if (! m_dataEventHandler) {
        m_dataEventHandler = eventHandler;
        m_dataEventHandlerUserData = ud;
        m_continuousReading = true;
        xTaskCreate(&esp32_c3_ADC::eventTask, "eventTaskADC", 4000, this, 2 | portPRIVILEGE_BIT, NULL);
        ok = true;
    }
    xSemaphoreGive(m_eventHandlerSemaphore);
    return ok;
}

void esp32_c3_ADC::continuousAdcInit(uint16_t adc1_chan_mask, adc1_channel_t *channel, uint8_t channel_num)
{
    esp_err_t ret = ESP_OK;
    assert(ret == ESP_OK);

    adc_digi_init_config_t adc_dma_config = {
        .max_store_buf_size = 1024,
        .conv_num_each_intr = 256,
        .adc1_chan_mask = adc1_chan_mask,
        .adc2_chan_mask = 0x00,
    };
    ret = adc_digi_initialize(&adc_dma_config);
    assert(ret == ESP_OK);

    adc_digi_pattern_table_t adc_pattern[10] = {0};

    //Do not set the sampling frequency out of the range between `SOC_ADC_SAMPLE_FREQ_THRES_LOW` and `SOC_ADC_SAMPLE_FREQ_THRES_HIGH`
    adc_digi_config_t dig_cfg = {
        .conv_limit_en = 0,
        .conv_limit_num = 250,
        .sample_freq_hz = 44100,
    };

    dig_cfg.adc_pattern_len = channel_num;
    for (int i = 0; i < channel_num; i++) {
        uint8_t unit = ((channel[i] >> 3) & 0x1);
        uint8_t ch = channel[i] & 0x7;
        adc_pattern[i].atten = ADC_ATTEN_DB_0;
        adc_pattern[i].channel = ch;
        adc_pattern[i].unit = unit;
    }
    dig_cfg.adc_pattern = adc_pattern;
    ret = adc_digi_controller_config(&dig_cfg);
    assert(ret == ESP_OK);
    
   
            
}
bool esp32_c3_ADC::checkValidData(const adc_digi_output_data_t *data)
{
    const unsigned int unit = data->type2.unit;
    if (unit > 2) return false;
    if (data->type2.channel >= SOC_ADC_CHANNEL_NUM(unit)) return false;

    return true;
}


void esp32_c3_ADC::eventTask(void *arg) 
{
    esp32_c3_ADC* myself = (esp32_c3_ADC*) arg;
    esp_err_t ret;
    uint32_t ret_num = 0;
    uint8_t result[TIMES] = {0};
    int16_t rawData[TIMES / 4] = {0};
    memset(result, 0xcc, TIMES);
    memset(rawData, 0xcc, TIMES / 2);
    uint16_t adc1_chan_mask = BIT(0);
    adc1_channel_t channel[1] = {ADC1_CHANNEL_0};

    myself->continuousAdcInit(adc1_chan_mask, channel, sizeof(channel) / sizeof(adc_channel_t));
    adc_digi_start();

    while(myself->m_continuousReading) {
        ret = adc_digi_read_bytes(result, TIMES, &ret_num, ADC_MAX_DELAY);
        for (int i = 0; i < ret_num; i+=4) {
            adc_digi_output_data_t *p = (adc_digi_output_data_t*)&result[i];
            if (checkValidData(p)) {
                rawData[i / 4] = ((int16_t)(p->type2.data << 4));
            } else {
                rawData[i / 4] = 0xFFFF;
            }
        }
            
        xSemaphoreTake(myself->m_eventHandlerSemaphore, portMAX_DELAY);
        if (myself->m_dataEventHandler) myself->m_dataEventHandler(rawData, ret_num / 2, myself->m_dataEventHandlerUserData);
        xSemaphoreGive(myself->m_eventHandlerSemaphore);
            
        
    }

    adc_digi_stop();
    adc_digi_deinitialize();
    
    vTaskDelete(NULL);
}

