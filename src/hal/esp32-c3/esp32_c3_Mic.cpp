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

void esp32_c3_Mic::dataStreamWorker(void* data, size_t size) {
    int16_t* dat = (int16_t*)data;
    if (m_noiceLevel != REDUCTION_LEVEL_RAW) {
        // find the average
        for (int i = 1; i < size / 2; i++) {
            m_average += dat[i];
            m_averageCount++;
            

            // max 1 Sec. average
            if (m_average > 0x7FFF0000 || m_average < -0x7FFF0000 || m_averageCount == m_hz) {
                m_avg = m_average / m_averageCount;
                m_average = m_avg;
                m_averageCount = 1;

            }
            
        }
        if (m_avg == 0) m_avg = m_average / m_averageCount;
        // find the max sound Level
        bool nosound = true;
        for (int i = 0; i < size / 2; i++) {
            int16_t* d = (int16_t*)(&dat[i]);
            *d -= m_avg;
            if (*d > m_noiceReductionMinValue) {
                 nosound = false;
            } 
            
            
            
        }
        if (nosound) {
            for (int i = 0; i < size / 2; i++) dat[i] = 0;
        }


      
        if (m_volume != 100) {
            for (int i = 0; i < size / 2; i++) {
                int16_t* d = (int16_t*)(&dat[i]);
                register int32_t vol = *d;
                vol *= m_volume / 100;
                if (vol > 0x7FFF) vol = 0x7FFF;
                if (vol < -0x8000) vol = -0x8000;
                *d = (int16_t)vol;

            }
        }
    }
    if (m_eventHandler != NULL) {
        m_eventHandler(MIC_DATA_PACKAGE, data, size, m_eventHandlerUserData);
    }
}
void esp32_c3_Mic::dataStream(void* data, size_t size, void* userData) {
    if (size < 2) return;
    esp32_c3_Mic* myself = (esp32_c3_Mic*)userData;
    myself->dataStreamWorker(data, size);
    
}
PocuterMicrophone::MICERROR esp32_c3_Mic::startRecording(PocuterMicrophone::SAMLE_RATE_HZ hz, PocuterMicrophone::NOICE_REDUCTION_LEVEL n) {
    switch (n) {
        case REDUCTION_LEVEL_RAW:
        case REDUCTION_LEVEL_NONE:
            m_noiceReductionMinValue = 0;
            
            
            break;
        case REDUCTION_LEVEL_SMALL:
            m_noiceReductionMinValue = 0x0FF;
           
            
            break;
        case REDUCTION_LEVEL_MEDIUM:
            m_noiceReductionMinValue = 0x1FF;
            
            
            break;
        case REDUCTION_LEVEL_HIGHT:
            m_noiceReductionMinValue = 0xFFF;
           
            
            break;
            
    }
    m_hz = hz;
    m_noiceLevel = n;
    m_volume = 100;
    
    m_average = 0;
    m_averageCount = 0;
    m_avg = 0;
    
    
    if (esp32_c3_ADC::Instance()->registerContinuousRead(esp32_c3_Mic::dataStream, hz, this)) {
        return MICERROR_OK;
    }
    return MICERROR_RESOURCE_NOT_AVAILABLE;
}
PocuterMicrophone::MICERROR esp32_c3_Mic::stopRecording() {
    esp32_c3_ADC::Instance()->unregisterContinuousRead();
    return MICERROR_UNKNOWN;
}
            
#endif