/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   esp32_c3_LightSensor.h
 * Author: Test
 *
 * Created on 25. Februar 2022, 12:49
 */

#ifndef ESP32_C3_LIGHTSENSOR_H
#define ESP32_C3_LIGHTSENSOR_H
#include "include/hal/PocuterLightSensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace PocuterLib {
    namespace HAL {
        class esp32_c3_LightSensor : public PocuterLightSensor {
        public:
            
            
            esp32_c3_LightSensor();
            uint16_t getValue();
            
            LIGHTERROR registerEventHandler(lightEventHandler*, uint16_t limit, uint16_t tollerance, void*);
            void unregisterEventHandler(); 
            
            virtual ~esp32_c3_LightSensor();
        private:
            static void eventTask(void *arg);
            
            lightEventHandler* m_lightEventHandler;
            void* m_lightEventHandlerUserData;
            uint16_t m_lightEventHandlerLimit;
            uint16_t m_lightEventHandlerTollerance;
            SemaphoreHandle_t m_eventHandlerSemaphore;
        };
    }
}

#endif /* ESP32_C3_LIGHTSENSOR_H */

