#include "include/hal/PocuterMicrophone.h"
#include <stddef.h>


#ifndef ESP32_C3_MIC_H
#define ESP32_C3_MIC_H
namespace PocuterLib {
    namespace HAL {
        class esp32_c3_Mic : public PocuterMicrophone{
        public:
            esp32_c3_Mic();
            MICERROR registerEventHandler(micEventHandler*, void*);
            void unregisterEventHandler();
            MICERROR startRecording();
            MICERROR stopRecording();
            virtual ~esp32_c3_Mic();
        private:
            static void dataStream(void* data, size_t size, void* userData);
            micEventHandler* m_eventHandler;
            void* m_eventHandlerUserData;
        };
    }
}

#endif /* ESP32_C3_MIC_H */

