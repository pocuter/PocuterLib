#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_MICROPHONE
#include "include/hal/esp32-c3/esp32_c3_Mic.h"
#include "include/hal/esp32-c3/esp32_c3_ADC.h"

using namespace PocuterLib::HAL;
esp32_c3_Mic::esp32_c3_Mic() {
    m_eventHandler = NULL;
}


esp32_c3_Mic::~esp32_c3_Mic() {
}

PocuterMicrophone::MICERROR esp32_c3_Mic::registerEventHandler(micEventHandler* f, void* ud) {
    if (m_eventHandler == NULL) {
        m_eventHandler = f;
        m_eventHandlerUserData = ud;
        return MICERROR_UNKNOWN;
    }
    return MICERROR_RESOURCE_NOT_AVAILABLE;
}
void esp32_c3_Mic::unregisterEventHandler() {
    m_eventHandler = NULL;
}
void esp32_c3_Mic::dataStream(void* data, size_t size, void* userData) {
    esp32_c3_Mic* myself = (esp32_c3_Mic*)userData;
    if (myself->m_eventHandler != NULL) {
        myself->m_eventHandler(MIC_DATA_PACKAGE, data, size, myself->m_eventHandlerUserData);
    }
}
PocuterMicrophone::MICERROR esp32_c3_Mic::startRecording() {
    if (esp32_c3_ADC::Instance()->registerContinuousRead(esp32_c3_Mic::dataStream, this)) {
        return MICERROR_OK;
    }
    return MICERROR_RESOURCE_NOT_AVAILABLE;
}
PocuterMicrophone::MICERROR esp32_c3_Mic::stopRecording() {
    esp32_c3_ADC::Instance()->unregisterContinuousRead();
    return MICERROR_UNKNOWN;
}
            
#endif