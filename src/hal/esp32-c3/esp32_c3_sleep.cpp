#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_SLEEP

#include "include/hal/esp32-c3/esp32_c3_sleep.h"


#include "esp_sleep.h"
#include "include/hal/esp32-c3/esp32_c3_Expander.h"
#include "include/hal/PocuterConfig.h"


using namespace PocuterLib::HAL;

TimerHandle_t esp32_c3_sleep::m_timer;
esp32_c3_sleep::SLEEPTIMER_INTERRUPTS esp32_c3_sleep::s_sleepTimerInterruptCause = 0;

esp32_c3_sleep::esp32_c3_sleep(PocuterDisplay* display, PocuterPorts* ports, PocuterAccelerometer* accl) {
    m_sm = SLEEP_MODE_LIGHT;
    m_wakeUpModes = WAKEUP_MODE_ANY_BUTTON;
    m_display = display;
    m_ports = ports;
    m_accl = accl;
    m_sleepEventHandler = NULL;
    m_inacTimerSec = 0;
    m_timer = xTimerCreate(
      "sleepTimer",
      pdMS_TO_TICKS(1000 * 300),
      pdFALSE,
      this,
      esp32_c3_sleep::sleepTimerCallback); 
     
}


esp32_c3_sleep::~esp32_c3_sleep() {
}

void esp32_c3_sleep::registerEventHandler(sleepEventHandler* e, void* u) {
    m_sleepEventHandler = e;
    m_sleepEventHandlerUserData = u;
}
void esp32_c3_sleep::unregisterEventHandler() {
    m_sleepEventHandler = NULL;
}
            

PocuterSleep::SLEEPERROR esp32_c3_sleep::setSleepMode(SLEEP_MODE sm) {
    m_sm = sm;
    m_wakeUpModes = 0x00;
    return SLEEPERROR_OK;
}
PocuterSleep::SLEEPERROR esp32_c3_sleep::setWakeUpModes(WAKEUP_MODES wm) {
    if (m_sm != SLEEP_MODE_LIGHT && (wm & WAKEUP_MODE_ANY_BUTTON)) return SLEEPERROR_MODE_NOT_POSSIBLE;
    if (m_sm != SLEEP_MODE_LIGHT && (wm & WAKEUP_MODE_SHAKE)) return SLEEPERROR_MODE_NOT_POSSIBLE;
    if (m_sm != SLEEP_MODE_LIGHT && (wm & WAKEUP_MODE_WIFI)) return SLEEPERROR_MODE_NOT_POSSIBLE;
    if ((m_wakeUpModes & WAKEUP_MODE_USERPORT_0_LOW) && (m_wakeUpModes & WAKEUP_MODE_USERPORT_0_HIGH)) return SLEEPERROR_MODE_NOT_POSSIBLE;
    if ((m_wakeUpModes & WAKEUP_MODE_USERPORT_5_LOW) && (m_wakeUpModes & WAKEUP_MODE_USERPORT_5_HIGH)) return SLEEPERROR_MODE_NOT_POSSIBLE;
    
    m_wakeUpModes = wm;
    return SLEEPERROR_OK;
}
PocuterSleep::SLEEPERROR esp32_c3_sleep::setWakeUpTimer(uint32_t sec) {
    m_sleepTimerMs = sec * 1000;
    return SLEEPERROR_OK;
}
void esp32_c3_sleep::resetSleepTimer(SLEEPTIMER_INTERRUPT cause) {
    if (! (cause & s_sleepTimerInterruptCause)) return;
    if (xTimerIsTimerActive(m_timer)) {
        xTimerReset(m_timer, portMAX_DELAY);
    }
}
PocuterSleep::SLEEPERROR esp32_c3_sleep::setInactivitySleep(uint32_t sec, SLEEPTIMER_INTERRUPTS cause, bool saveTimeout) {
    s_sleepTimerInterruptCause = cause;
    m_inacTimerSec = sec;
    if (sec == 0 && xTimerIsTimerActive(m_timer)) {
        xTimerStop(m_timer, portMAX_DELAY);
    }
    if (sec > 0) {
        xTimerChangePeriod(m_timer, pdMS_TO_TICKS(1000 * sec), portMAX_DELAY);
        xTimerReset(m_timer, portMAX_DELAY);
    }
    if (saveTimeout) {
        uint64_t appId = 1;
        PocuterConfig* config = new PocuterConfig((const uint8_t*)"INACT_SLEEP", &appId);
        config->set((const uint8_t*)"SLEEP", (const uint8_t*)"timeout", sec);
        delete(config);
    }
    
    return SLEEPERROR_OK;
}
void esp32_c3_sleep::sleepTimerCallback(TimerHandle_t xTimer) {
    esp32_c3_sleep* myself = (esp32_c3_sleep*)pvTimerGetTimerID(xTimer);
    myself->doSleepNowEx(true);
}
PocuterSleep::SLEEPERROR esp32_c3_sleep::doSleepNow() {
    return doSleepNowEx(false);
}


PocuterSleep::SLEEPERROR esp32_c3_sleep::doSleepNowEx(bool fromTimer) {
    if (fromTimer && m_sleepEventHandler){
        bool sleepOkay = m_sleepEventHandler(SLEEP_EVENT_ENTER_SLEEP, m_sleepEventHandlerUserData);
        if (! sleepOkay){
            xTimerChangePeriod(m_timer, pdMS_TO_TICKS(1000 * m_inacTimerSec), portMAX_DELAY);
            xTimerReset(m_timer, portMAX_DELAY);
            return SLEEPERROR_SLEEP_INTERRUPTED;
        }
    }
    if (m_ports != NULL) m_ports->pauseInterruptHandler();
    if (m_accl != NULL) m_accl->pauseInterruptHandler();
    esp32_c3_Expander::Instance()->pauseInterruptHandler();
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    
    if (m_sm == SLEEP_MODE_DEEP && ((m_wakeUpModes & WAKEUP_MODE_USERPORT_0_LOW) || (m_wakeUpModes & WAKEUP_MODE_USERPORT_0_HIGH) || (m_wakeUpModes & WAKEUP_MODE_USERPORT_5_LOW) || (m_wakeUpModes & WAKEUP_MODE_USERPORT_5_HIGH))) {
        uint64_t maskLow = 0;
        uint64_t maskHigh = 0;
        if (m_wakeUpModes & WAKEUP_MODE_USERPORT_0_LOW)  maskLow |= 0x10;
        if (m_wakeUpModes & WAKEUP_MODE_USERPORT_0_HIGH)  maskHigh |= 0x10;
        if (m_wakeUpModes & WAKEUP_MODE_USERPORT_5_LOW)  maskLow |= 0x08;
        if (m_wakeUpModes & WAKEUP_MODE_USERPORT_5_HIGH)  maskHigh |= 0x08;
        
        const gpio_config_t config = {
        .pin_bit_mask = maskLow | maskHigh,
        .mode = GPIO_MODE_INPUT,
        };
        gpio_config(&config);
        esp_err_t err = ESP_OK;
        if (maskLow) err = esp_deep_sleep_enable_gpio_wakeup(maskLow, ESP_GPIO_WAKEUP_GPIO_LOW);
        if (err == ESP_OK && maskHigh) err = esp_deep_sleep_enable_gpio_wakeup(maskHigh, ESP_GPIO_WAKEUP_GPIO_HIGH);
        if (err != ESP_OK) return SLEEPERROR_COULD_NOT_SET_GPIO_WAKEUP;
        
    }
    
    
    if (m_sm == SLEEP_MODE_LIGHT && ((m_wakeUpModes & WAKEUP_MODE_USERPORT_0_LOW) || (m_wakeUpModes & WAKEUP_MODE_USERPORT_0_HIGH) || (m_wakeUpModes & WAKEUP_MODE_USERPORT_5_LOW) || (m_wakeUpModes & WAKEUP_MODE_USERPORT_5_HIGH) || (m_wakeUpModes & WAKEUP_MODE_ANY_BUTTON))) {
        
        if (m_wakeUpModes & WAKEUP_MODE_USERPORT_0_LOW) gpio_wakeup_enable(GPIO_NUM_4, GPIO_INTR_LOW_LEVEL);
        if (m_wakeUpModes & WAKEUP_MODE_USERPORT_0_HIGH) gpio_wakeup_enable(GPIO_NUM_4, GPIO_INTR_HIGH_LEVEL);
        if (m_wakeUpModes & WAKEUP_MODE_USERPORT_5_LOW) gpio_wakeup_enable(GPIO_NUM_3, GPIO_INTR_LOW_LEVEL);
        if (m_wakeUpModes & WAKEUP_MODE_USERPORT_5_HIGH) gpio_wakeup_enable(GPIO_NUM_3, GPIO_INTR_HIGH_LEVEL);
        
        if (m_wakeUpModes & WAKEUP_MODE_ANY_BUTTON){
            gpio_wakeup_enable(GPIO_NUM_20, GPIO_INTR_LOW_LEVEL);
        }

        if (m_wakeUpModes & WAKEUP_MODE_SHAKE){
            gpio_wakeup_enable(GPIO_NUM_7, GPIO_INTR_LOW_LEVEL);
        }

        esp_err_t err = esp_sleep_enable_gpio_wakeup();
        if (err != ESP_OK) return SLEEPERROR_COULD_NOT_SET_GPIO_WAKEUP;
        
    }
    if (m_wakeUpModes & WAKEUP_MODE_TIMER) {
       esp_err_t err = esp_sleep_enable_timer_wakeup(m_sleepTimerMs*1000);
       if (err != ESP_OK) return SLEEPERROR_COULD_NOT_SET_TIMER_WAKEUP;
    } else {
    	// with arduino it seems to crash on wakeup if i didn't set a timer. 
    	esp_sleep_enable_timer_wakeup(0xFFFFFFFFFFFF); //nearly 9000 years
    }
    if (m_wakeUpModes & WAKEUP_MODE_WIFI) {
       esp_err_t err = esp_sleep_enable_wifi_wakeup();
       if (err != ESP_OK) return SLEEPERROR_COULD_NOT_SET_WLAN_WAKEUP;
    } else {
        esp_sleep_disable_wifi_wakeup();
    }
    if (m_sm == SLEEP_MODE_DEEP) {
#ifndef POCUTER_DISABLE_DISPLAY             
          if (m_display != NULL) {
              m_display->doSleep();
          }
#endif
        esp_deep_sleep_start();
        // he will never come to this point
    }
    if (m_sm == SLEEP_MODE_LIGHT) {
        m_display->doSleep();
        esp_light_sleep_start();
        
        if (m_wakeUpModes & WAKEUP_MODE_USERPORT_0_LOW || m_wakeUpModes & WAKEUP_MODE_USERPORT_0_HIGH) gpio_wakeup_disable(GPIO_NUM_4);
        if (m_wakeUpModes & WAKEUP_MODE_USERPORT_5_LOW || m_wakeUpModes & WAKEUP_MODE_USERPORT_5_HIGH) gpio_wakeup_disable(GPIO_NUM_3);
        if (m_wakeUpModes & WAKEUP_MODE_ANY_BUTTON) gpio_wakeup_disable(GPIO_NUM_20);
        
        m_display->doWakeUp();
        if (m_ports != NULL) m_ports->resumeInterruptHandler();
        if (m_accl != NULL) m_accl->resumeInterruptHandler();
        esp32_c3_Expander::Instance()->resumeInterruptHandler();
        if (fromTimer) {
            if (m_sleepEventHandler){
                bool wakeUpOkay = m_sleepEventHandler(SLEEP_EVENT_WAKEUP, m_sleepEventHandlerUserData);
                if (wakeUpOkay) {
                    xTimerChangePeriod(m_timer, pdMS_TO_TICKS(1000 * m_inacTimerSec), portMAX_DELAY);
                } else {
                    xTimerChangePeriod(m_timer, pdMS_TO_TICKS(10), portMAX_DELAY); // we should sleep immediately again.
                }
            } else {
                xTimerChangePeriod(m_timer, pdMS_TO_TICKS(1000 * m_inacTimerSec), portMAX_DELAY);
            }

            xTimerReset(m_timer, portMAX_DELAY);
        }
    }
    
    return SLEEPERROR_OK;
}
PocuterSleep::WAKEUP_CAUSE esp32_c3_sleep::getWakeUpCause() {
   esp_sleep_wakeup_cause_t  c = esp_sleep_get_wakeup_cause();
   
   if (c == ESP_SLEEP_WAKEUP_TIMER) return WAKEUP_CAUSE_TIMER;
   if (c == ESP_SLEEP_WAKEUP_GPIO) return WAKEUP_CAUSE_GPIO;
   if (c == ESP_SLEEP_WAKEUP_WIFI) return WAKEUP_CAUSE_WIFI;
   return  WAKEUP_CAUSE_POWER_ON;
}



#endif      