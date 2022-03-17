
#ifndef ESP32_C3_ADC_H
#define ESP32_C3_ADC_H

#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
namespace PocuterLib {
    namespace HAL {
        class esp32_c3_ADC {
        public:
            
            static int singleRead(adc1_channel_t);
            static esp32_c3_ADC* Instance(void);
            typedef void (adcEventHandlerDataEvailable)(void* data, size_t size, void* userData);
            
            bool registerContinuousRead(adcEventHandlerDataEvailable* eventHandler, uint32_t sampleFreqHZ, void*);
            bool unregisterContinuousRead();
            
        private:
            static esp32_c3_ADC* m_pInstance;
            
            
            esp32_c3_ADC();
            virtual ~esp32_c3_ADC();
            void continuousAdcInit(uint16_t adc1_chan_mask, adc1_channel_t *channel, uint8_t channel_num, uint32_t sampleFreqHZ);
            void continuousRead();
            static bool checkValidData(const adc_digi_output_data_t *data);
            static void eventTask(void *arg);
            bool m_continuousReading;
            adcEventHandlerDataEvailable* m_dataEventHandler;
            void* m_dataEventHandlerUserData;
            SemaphoreHandle_t m_eventHandlerSemaphore;
            uint32_t m_sampleFreqHZ;
            
        };
    }
}

#endif /* ESP32_C3_ADC_H */

