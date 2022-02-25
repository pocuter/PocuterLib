#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_LIGHTSENSOR
#include "include/hal/esp32-c3/esp32_c3_LightSensor.h"
#include "include/hal/esp32-c3/esp32_c3_ADC.h"

#define SENSOR_GPIO ADC1_CHANNEL_1
using namespace PocuterLib::HAL;
/*
 Will will only generate the polling thread, if someone registered the event handler. If unregisterEventHandler is called the thread will stop.
 
 */
esp32_c3_LightSensor::esp32_c3_LightSensor() {
    m_lightEventHandler = NULL;
    m_eventHandlerSemaphore = xSemaphoreCreateBinary();
    if (!m_eventHandlerSemaphore) abort();
    xSemaphoreGive(m_eventHandlerSemaphore);
}


esp32_c3_LightSensor::~esp32_c3_LightSensor() {
}

uint16_t esp32_c3_LightSensor::getValue() {
    int r = esp32_c3_ADC::singleRead(SENSOR_GPIO);
    return r;
}

PocuterLightSensor::LIGHTERROR esp32_c3_LightSensor::registerEventHandler(lightEventHandler* e, uint16_t limit, uint16_t tollerance, void* u) {
    xSemaphoreTake(m_eventHandlerSemaphore, portMAX_DELAY);
    if (m_lightEventHandler == NULL) xTaskCreate(&eventTask, "eventTaskPocuterLight", 4000, this, 10, NULL);
    m_lightEventHandler = e;
    m_lightEventHandlerUserData = u;
    m_lightEventHandlerLimit = limit;
    m_lightEventHandlerTollerance = tollerance;
    xSemaphoreGive(m_eventHandlerSemaphore);
    return LIGHTERROR_OK;
}
void esp32_c3_LightSensor::unregisterEventHandler() {
    xSemaphoreTake(m_eventHandlerSemaphore, portMAX_DELAY);
    m_lightEventHandler = NULL;
    xSemaphoreGive(m_eventHandlerSemaphore);
    vTaskDelay(configTICK_RATE_HZ / 150);
}

void esp32_c3_LightSensor::eventTask(void *arg) {
    esp32_c3_LightSensor* myself = (esp32_c3_LightSensor*)arg;
    
    uint8_t isAbove = 2; // to give it one first shot
    uint16_t lastTLightValue = myself->getValue();
    xSemaphoreTake(myself->m_eventHandlerSemaphore, portMAX_DELAY);
    bool hasEventHandler = myself->m_lightEventHandler;
    xSemaphoreGive(myself->m_eventHandlerSemaphore);
    uint16_t tolerance = 0;
    while(hasEventHandler) {
        xSemaphoreTake(myself->m_eventHandlerSemaphore, portMAX_DELAY);
        hasEventHandler = myself->m_lightEventHandler;
        uint16_t value = myself->getValue();
        if (isAbove && value + tolerance <= myself->m_lightEventHandlerLimit) {
            isAbove = 0;
            if (hasEventHandler) myself->m_lightEventHandler(LIGHT_UNDER_LIMIT, myself->m_lightEventHandlerUserData);
            
        }
        if (isAbove != 1 && value - tolerance > myself->m_lightEventHandlerLimit) {
            isAbove = 1;
            if (hasEventHandler) myself->m_lightEventHandler(LIGHT_ABOVE_LIMIT, myself->m_lightEventHandlerUserData);
        }
        if (value > lastTLightValue + tolerance || value < lastTLightValue - tolerance) {
            lastTLightValue = value;
            if (hasEventHandler) myself->m_lightEventHandler(LIGHT_TOLLERANCE_CHANGED, myself->m_lightEventHandlerUserData);
        }
        tolerance = myself->m_lightEventHandlerTollerance;
        xSemaphoreGive(myself->m_eventHandlerSemaphore);
        vTaskDelay(configTICK_RATE_HZ / 50);
        
    }
    vTaskDelete(NULL);
}        
#endif
