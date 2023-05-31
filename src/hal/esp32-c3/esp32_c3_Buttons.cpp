#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_BUTTONS

#include "include/hal/esp32-c3/esp32_c3_Buttons.h"
#include "include/hal/esp32-c3/esp32_c3_WIFI.h"
#include "include/hal/esp32-c3/esp32_c3_sleep.h"
#include "Pocuter.h"
#include "include/hal/PocuterDeviceType.h"

#include "include/PocuterLibConfig.h"

#include "include/hal/esp32-c3/esp32_c3_sleep.h"

#include "Pocuter.h"

#define ACTIVATED_REPEAT_TIME_FIRST 500
#define ACTIVATED_REPEAT_TIME       100
#define LONG_HOLD_TIME              1000
using namespace PocuterLib::HAL;


const esp32_c3_Buttons::BUTTON_INFO esp32_c3_Buttons::s_buttonInfo_pocuter_ext[BUTTON_NUM] = {
    // exp  port pin mirror
    {true,  0, 6, -1},
    {true,  0, 5, -1},
    {true,  0, 1, -1},
#ifdef POCUTER_EXTENDED_BUTTONS    
    {true,  1, 1, -1},
    {true,  1, 0, -1},
    {false, 0, 4, -1},
    {true,  1, 7, -1},
    {true,  1, 6, -1},
    {false, 0, 3, -1}
#endif    
};

const esp32_c3_Buttons::BUTTON_INFO esp32_c3_Buttons::s_buttonInfo_pocket[10] = {
    //BUTTON_1 = BUTTON_UP, BUTTON_2 = BUTTON_DOWN, BUTTON_3 = BUTTON_A for Pocuter compatibility
    // exp  port pin  mirror
    {true,  1, 2, -1},
    {true,  1, 0, -1},
    {true,  0, 5, -1},
    {true,  1, 2,  0},
    {true,  1, 0,  1},
    {true,  1, 3, -1},
    {true,  1, 1, -1},
    {true,  0, 5,  2},
    {false, 0, 9, -1},
    {false, 0, 2, -1}
};
/*  
    {true,  1, 2},
    {true,  1, 0},
    {true,  0, 5},
    {true,  1, 2},
    {true,  1, 0},
    {true,  1, 3},
    {true,  1, 1},
    {true,  0, 5},
    {false, 0, 9}
    {false, 0, 2},
   
*/
esp32_c3_Buttons::esp32_c3_Buttons() {
    m_expander = esp32_c3_Expander::Instance();
    m_stopForwardingKeys = false;
    m_buttonCount = 0;
    if (PocuterDeviceType::deviceType == PocuterDeviceType::DEVICE_TYPE_POCKETSTAR_2){
        m_binfo = s_buttonInfo_pocket;
        m_buttonCount = 10;
    } else {
        m_binfo = s_buttonInfo_pocuter_ext;
#ifdef POCUTER_EXTENDED_BUTTONS
        m_buttonCount = 9;
#else
        m_buttonCount = 3;
#endif
        
    }
    
    
    for (int i = 0; i < m_buttonCount; i++) {
        BUTTON_INFO info = m_binfo[i];
        if (info.mirror != -1 ) continue; 
        if (info.isExpander) {
            m_expander->pinMode(info.port, info.pin, EXPANDER_IN);
            m_expander->registerInterruptPin(info.port, info.pin);
            
            //m_expander->registerInterruptPin(info.port, info.pin);
        } else {
            // following lines of code are copied from PocuterPorts::initPort
            gpio_config_t io_conf = {};
            io_conf.intr_type = GPIO_INTR_DISABLE;
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pin_bit_mask = (1ULL<<info.pin);
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
            gpio_config(&io_conf);
        }
    }
    
    for (int i = 0; i < m_buttonCount; i++) {
        BUTTON_HELPER *button = &m_buttonHelper[i];
        button->status = (PBACTION) 0;
        button->wasPressed = false;
    }
    
    
}

void esp32_c3_Buttons::updateButtons() {
    uint64_t ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
    
    for (int i = 0; i < m_buttonCount; i++) {
        const BUTTON_INFO *info = &m_binfo[i];
        BUTTON_HELPER *button = &m_buttonHelper[i];
        
        
        if (info->mirror != -1) {
            *button = m_buttonHelper[info->mirror];
            continue;
        }
        
        bool pressed;
        
        if (info->isExpander) {
            pressed = (m_expander->readPin(info->port, info->pin)) ? false : true;
        } else {
            pressed = !gpio_get_level((gpio_num_t)info->pin);
        }
        
        uint8_t newStatus = 0;
        
        if (!button->wasPressed && pressed) {
            newStatus |= BUTTON_PRESSED | BUTTON_ACTIVATED | BUTTON_HELD;
            button->lastTimeActivated = ms;
            button->pressTime = ms;
            button->firstActivated = true;
        }
        
        if (button->wasPressed && !pressed) {
            
            if (PocuterDeviceType::deviceType == PocuterDeviceType::DEVICE_TYPE_POCKETSTAR_2 && i == BUTTON_NUM_HOME && ms - button->pressTime > LONG_HOLD_TIME){
                // do shut down now (we have to wait until the button is released again, because he would wake up immediately otherwise 
                esp32_c3_sleep::shutDown(false);
            }
                    
            newStatus |= BUTTON_RELEASED;
        }
        
        if (button->wasPressed && pressed) {
            newStatus |= BUTTON_HELD;
            if (button->firstActivated) {
                if (ms - button->lastTimeActivated > ACTIVATED_REPEAT_TIME_FIRST) {
                    newStatus |= BUTTON_ACTIVATED;
                    button->lastTimeActivated += ACTIVATED_REPEAT_TIME_FIRST;
                    button->firstActivated = false;
                    
                }
            } else {
                if (ms - button->lastTimeActivated > ACTIVATED_REPEAT_TIME) {
                    newStatus |= BUTTON_ACTIVATED;
                    button->lastTimeActivated += ACTIVATED_REPEAT_TIME;
                }
            }
            if (PocuterDeviceType::deviceType == PocuterDeviceType::DEVICE_TYPE_POCKETSTAR_2 && i == BUTTON_NUM_HOME && ms - button->pressTime > LONG_HOLD_TIME){
                // we only shutdown the display. Full shut down after this button was released
                esp32_c3_sleep::shutDown(true);
            }
        }
        if (newStatus & BUTTON_PRESSED) esp32_c3_sleep::resetSleepTimer(esp32_c3_sleep::SLEEPTIMER_INTERRUPT_BY_BUTTON);
        button->wasPressed = pressed;
        button->status = (PBACTION) newStatus;
    }
}

PocuterButtons::PBACTION esp32_c3_Buttons::getButtonStatus(PBUTTON button) {
    if (m_stopForwardingKeys) return (PocuterButtons::PBACTION)0;
    if (button >= m_buttonCount)
        return (PBACTION) 0;
    return m_buttonHelper[button].status;
}

bool esp32_c3_Buttons::isButtonPressedDown(PBUTTON button) {
    if (m_stopForwardingKeys) return false;
    return isButtonPressedDownInternal(button);
}

PocuterButtons::PBUTTONS esp32_c3_Buttons::getButtonState() {
    updateButtons();
    if (m_stopForwardingKeys) return (PocuterButtons::PBUTTONS)0;
    int b = 0;
    
    
    for (int i = 0; i < m_buttonCount; i++) {
        
        BUTTON_HELPER *button = &m_buttonHelper[i];
        if (button->wasPressed) b |= 1 << i;
    }
    return (PocuterButtons::PBUTTONS)b;
}


bool esp32_c3_Buttons::isButtonPressedDownInternal(PBUTTON button) {
    if (button >= m_buttonCount)
        return 0;
    const BUTTON_INFO* info = &m_binfo[button];
    if (info->isExpander) {
        return (m_expander->readPin(info->port, info->pin)) ? false : true;
    } else {
        return !gpio_get_level((gpio_num_t)info->pin);
    }
}
void esp32_c3_Buttons::stopForwardingKeys() {
    m_stopForwardingKeys = true;
}
void esp32_c3_Buttons::continueForwardingKeys(){
    m_stopForwardingKeys = false;
}
            
            
esp32_c3_Buttons::~esp32_c3_Buttons() {
}
#endif

