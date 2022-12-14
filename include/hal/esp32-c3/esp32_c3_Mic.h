#include "include/hal/PocuterMicrophone.h"
#include <stddef.h>
#include <stdint.h>


#ifndef ESP32_C3_MIC_H
#define ESP32_C3_MIC_H
namespace PocuterLib {
    namespace HAL {
        class esp32_c3_Mic : public PocuterMicrophone{
        public:
            esp32_c3_Mic();
            MICERROR registerEventHandler(micEventHandler*, void*);
            void unregisterEventHandler();
            MICERROR startRecording(PocuterMicrophone::SAMLE_RATE_HZ, PocuterMicrophone::NOICE_REDUCTION_LEVEL);
            MICERROR stopRecording();
            virtual ~esp32_c3_Mic();
        private:
            static void dataStream(void* data, size_t size, void* userData);
            void dataStreamWorker(void* data, size_t size);
            micEventHandler* m_eventHandler;
            void* m_eventHandlerUserData;
            uint8_t m_volume;
            int64_t m_average;
            uint32_t m_averageCount;
            int16_t m_avg;
            uint32_t m_hz;
  
            PocuterMicrophone::NOICE_REDUCTION_LEVEL m_noiceLevel;
            uint16_t m_noiceReductionMinValue;
            
        };
    }
}

#endif /* ESP32_C3_MIC_H */

