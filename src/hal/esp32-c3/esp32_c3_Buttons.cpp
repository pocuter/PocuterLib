#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_BUTTONS

#include "include/hal/esp32-c3/esp32_c3_Buttons.h"
#include "include/hal/esp32-c3/esp32_c3_WIFI.h"
#include "Pocuter.h"

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
PocuterButtons::PBUTTONS esp32_c3_Buttons::getButtonState() {
    uint8_t buttons = 0;
    if (! m_expander->readPin(EXP_PORT_BTN1, EXP_PIN_BTN1)) buttons |= BUTTON_1;
    if (! m_expander->readPin(EXP_PORT_BTN2, EXP_PIN_BTN2)) buttons |= BUTTON_2;
    if (! m_expander->readPin(EXP_PORT_BTN3, EXP_PIN_BTN3)) buttons |= BUTTON_3;
    return (PocuterButtons::PBUTTONS)buttons;
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
    
    uint8_t buttons = 0;
    uint8_t port = 0;
    
    if (EXP_PORT_BTN1 == 0) port = a; else port = b;
    if (!(port & (1 << EXP_PIN_BTN1))) buttons |= BUTTON_1;
    if (EXP_PORT_BTN2 == 0) port = a; else port = b;
    if (!(port & (1 << EXP_PIN_BTN2))) buttons |= BUTTON_2;
    if (EXP_PORT_BTN3 == 0) port = a; else port = b;
    if (!(port & (1 << EXP_PIN_BTN3))) buttons |= BUTTON_3;
    
    if (buttons != myself->m_currentButtonStates) {
        myself->checkCeatCodes(buttons);
        
        myself->m_currentButtonStates = buttons;
        if (myself->m_eventHandler != NULL) myself->m_eventHandler((PBUTTONS)buttons, myself->m_eventHandlerUserData);
    }
    
}
void esp32_c3_Buttons::checkCeatCodes(uint8_t buttons) {
#ifndef POCUTER_DISABLE_WIFI 
    // check for WIFI AP
    //if (buttons == (BUTTON_1 & BUTTON_2 & BUTTON_3)) {
        Pocuter::WIFI->startAccessPoint();
     //       Pocuter::WIFI->startWPS();
   // }
#endif
}
esp32_c3_Buttons::~esp32_c3_Buttons() {
}
#endif

