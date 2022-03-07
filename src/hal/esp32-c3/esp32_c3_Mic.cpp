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
            m_maxCounter++;

            // max 1 Sec. average
            if (m_average > 0x7FFF0000 || m_average < -0x7FFF0000 || m_averageCount == m_hz) {
                m_avg = m_average / m_averageCount;
                m_average = m_avg;
                m_averageCount = 1;

            }
            // reduce max sound level every xth sec.
            if (m_maxCounter == m_hz / m_noiceReductionXThSec) {
                m_maxCounter = 0;
                m_max /= m_noiceReduceSpeed;
            }
        }
        if (m_avg == 0) m_avg = m_average / m_averageCount;
        // find the max sound Level
        for (int i = 0; i < size / 2; i++) {
            int16_t* d = (int16_t*)(&dat[i]);
            *d -= m_avg;
            register int32_t m = dat[i];
            if (m < 0) m *= -1;
            m *= 1.5;
            if (m < 0x7FFF && m > m_max) m_max = m;
        }


        double dob;
        if (m_max > m_noiceReductionMinValue) {
            dob = 0x7FFF / m_max;
        } else {
            dob = 1;
        }
    
        for (int i = 0; i < size / 2; i++) {
            int16_t* d = (int16_t*)(&dat[i]);
            register int32_t vol = *d * dob;
            vol *= m_volume / 100;
            if (vol > 0x7FFF) vol = 0x7FFF;
            if (vol < -0x8000) vol = -0x8000;
            *d = (int16_t)vol;

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
            m_noiceReductionXThSec = 1;
            m_noiceReduceSpeed = 2;
            break;
        case REDUCTION_LEVEL_SMALL:
            m_noiceReductionMinValue = 0x1FF;
            m_noiceReductionXThSec = 3;
            m_noiceReduceSpeed = 1.1;
            break;
        case REDUCTION_LEVEL_MEDIUM:
            m_noiceReductionMinValue = 0x2FF;
            m_noiceReductionXThSec = 10;
            m_noiceReduceSpeed = 1.3;
            break;
        case REDUCTION_LEVEL_HIGHT:
            m_noiceReductionMinValue = 0x3FF;
            m_noiceReductionXThSec = 20;
            m_noiceReduceSpeed = 5;
            break;
            
    }
    m_hz = hz;
    m_noiceLevel = n;
    m_volume = 100;
    
    m_average = 0;
    m_averageCount = 0;
    m_avg = 0;
    m_max = 0;
    m_maxCounter = 0;
    
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