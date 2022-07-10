#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_RGBled

#include "include/hal/esp32-c3/esp32_c3_RGBled.h"
#include "driver/gpio.h"
using namespace PocuterLib::HAL;

#define LED_R_PORT      1
#define LED_R_PIN       2
#define LED_G_PORT      1
#define LED_G_PIN       3
#define LED_B_PORT      0
#define LED_B_PIN       0
  


esp32_c3_RGBled::esp32_c3_RGBled() {
    
    
    m_expander = esp32_c3_Expander::Instance();
       
    m_expander->ledMode(LED_R_PORT, LED_R_PIN, 0);
    m_expander->ledMode(LED_G_PORT, LED_G_PIN, 0);
    m_expander->ledMode(LED_B_PORT, LED_B_PIN, 0);
    
    m_expander->setLed(LED_R_PORT, LED_R_PIN, 0);
    m_expander->setLed(LED_G_PORT, LED_G_PIN, 0);
    m_expander->setLed(LED_B_PORT, LED_B_PIN, 0);
    
   
}
esp32_c3_RGBled::~esp32_c3_RGBled() {
    
}
RGBled::LEDERROR esp32_c3_RGBled::setRGB(uint8_t ledNumber, uint8_t r, uint8_t g, uint8_t b) {
    m_expander->setLed(LED_R_PORT, LED_R_PIN, r);
    m_expander->setLed(LED_G_PORT, LED_G_PIN, g);
    m_expander->setLed(LED_B_PORT, LED_B_PIN, b);
    return LEDERROR_OK;
}
#endif
