#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_BUTTONS

#include "include/hal/esp32-c3/esp32_c3_Buttons.h"


#define EXP_PORT_BTN1 0
#define EXP_PIN_BTN1  6

#define EXP_PORT_BTN2 0
#define EXP_PIN_BTN2  5

#define EXP_PORT_BTN3 0
#define EXP_PIN_BTN3  1

using namespace PocuterLib::HAL;

esp32_c3_Buttons::esp32_c3_Buttons() {
    m_expander = esp32_c3_Expander::Instance();
    m_expander->pinMode(EXP_PORT_BTN1, EXP_PIN_BTN1, EXPANDER_IN);
    m_expander->pinMode(EXP_PORT_BTN2, EXP_PIN_BTN2, EXPANDER_IN);
    m_expander->pinMode(EXP_PORT_BTN3, EXP_PIN_BTN3, EXPANDER_IN);
    
    m_expander->registerInterruptPin(EXP_PORT_BTN1, EXP_PIN_BTN1);
    m_expander->registerInterruptPin(EXP_PORT_BTN2, EXP_PIN_BTN2);
    m_expander->registerInterruptPin(EXP_PORT_BTN3, EXP_PIN_BTN3);
    
    
    m_expander->registerEventHandler(esp32_c3_Buttons::expEventHandler, this);
    m_eventHandler = NULL;
    
    m_currentButtonStates = 0;
    
}
uint8_t esp32_c3_Buttons::getButtonState(uint8_t button) {
    if (button == 0) return m_expander->readPin(EXP_PORT_BTN1, EXP_PIN_BTN1);
    if (button == 1) return m_expander->readPin(EXP_PORT_BTN2, EXP_PIN_BTN2);
    if (button == 2) return m_expander->readPin(EXP_PORT_BTN3, EXP_PIN_BTN3);
    return 0;
}
void esp32_c3_Buttons::registerEventHandler(PocuterButtons::buttonEventHandler* e, void* d) {
    m_eventHandler = e;
    m_eventHandlerUserData = d;
}
void esp32_c3_Buttons::unregisterEventHandler() {
    m_eventHandler = NULL;
    m_eventHandlerUserData = NULL;
}
void esp32_c3_Buttons::expEventHandler(uint8_t a, uint8_t b, void* dat) {
    esp32_c3_Buttons* myself = (esp32_c3_Buttons*) dat;
    
    if (myself->m_eventHandler == NULL) return;
    
    uint8_t buttons = 0;
    uint8_t port = 0;
    
    if (EXP_PORT_BTN1 == 0) port = a; else port = b;
    if (!(port & (1 << EXP_PIN_BTN1))) buttons |= BUTTON_1;
    if (EXP_PORT_BTN2 == 0) port = a; else port = b;
    if (!(port & (1 << EXP_PIN_BTN2))) buttons |= BUTTON_2;
    if (EXP_PORT_BTN3 == 0) port = a; else port = b;
    if (!(port & (1 << EXP_PIN_BTN3))) buttons |= BUTTON_3;
    
    
    if (buttons != myself->m_currentButtonStates) {
        myself->m_currentButtonStates = buttons;
        myself->m_eventHandler((PBUTTONS)buttons, myself->m_eventHandlerUserData);
    }
    
}
esp32_c3_Buttons::~esp32_c3_Buttons() {
}
#endif

