#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_MICROPHONE
#include "include/hal/esp32-c3/esp32_c3_Mic.h"
#include "include/hal/esp32-c3/esp32_c3_ADC.h"

using namespace PocuterLib::HAL;
#define SAMPLE_FREQUENCE_HZ 44100

esp32_c3_Mic::esp32_c3_Mic() {
    m_eventHandler = NULL;
    m_volume = 50;
    m_average = 0;
    m_averageCount = 0;
    m_avg = 0;
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
    int16_t* dat = (int16_t*)data;
    
    
    for (int i = 1; i < size / 2; i++) {
        myself->m_average += dat[i];
        myself->m_averageCount++;
        if (myself->m_average > 0x7FFF0000 || myself->m_average < -0x7FFF0000 || myself->m_averageCount == SAMPLE_FREQUENCE_HZ) {
            myself->m_avg = myself->m_average / myself->m_averageCount;
            myself->m_average = myself->m_avg;
            myself->m_averageCount = 1;
        }
    }
    
    
    myself->m_avg = myself->m_average / myself->m_averageCount;
        
    for (int i = 0; i < size / 2; i++) {
        int16_t* d = (int16_t*)(&dat[i]);
        *d -= myself->m_avg;
        register int32_t vol = *d * (myself->m_volume / 10);
        if (vol > 0x7FFF) vol = 0x7FFF;
        if (vol < -0x7FFF) vol = -0x7FFF;
        *d = (int16_t)vol;
     
    }
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