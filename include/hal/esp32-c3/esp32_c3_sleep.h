#ifndef ESP32_C3_SLEEP_H
#define ESP32_C3_SLEEP_H
#include "freertos/FreeRTOS.h"
#include <freertos/timers.h>

#include "include/hal/PocuterSleep.h"
#include "include/hal/PocuterDisplay.h"
#include "include/hal/PocuterPorts.h"
#include "include/hal/PocuterAccelerometer.h"

namespace PocuterLib {
    namespace HAL {
        class esp32_c3_sleep : public PocuterSleep {
        public:
            esp32_c3_sleep(PocuterDisplay* display, PocuterPorts* ports, PocuterAccelerometer* accl);
            virtual ~esp32_c3_sleep();
            void registerEventHandler(sleepEventHandler* e, void* u);
            void unregisterEventHandler();
            SLEEPERROR setSleepMode(SLEEP_MODE sm);
            SLEEPERROR setWakeUpModes(WAKEUP_MODES wm);
            SLEEPERROR setWakeUpTimer(uint32_t sec);
            SLEEPERROR doSleepNow();
            SLEEPERROR setInactivitySleep(uint32_t sec, SLEEPTIMER_INTERRUPTS cause, bool saveTimeout = false);
            WAKEUP_CAUSE getWakeUpCause();
            
            
            static void resetSleepTimer(SLEEPTIMER_INTERRUPT cause);
            
        private:
            static void sleepTimerCallback(TimerHandle_t xTimer);
            SLEEP_MODE m_sm;
            WAKEUP_MODES m_wakeUpModes;
            uint32_t m_sleepTimerMs;
            PocuterDisplay* m_display;
            PocuterPorts* m_ports;
            PocuterAccelerometer* m_accl;
            static TimerHandle_t m_timer;
            static SLEEPTIMER_INTERRUPTS s_sleepTimerInterruptCause;
            sleepEventHandler* m_sleepEventHandler;
            void* m_sleepEventHandlerUserData;
            uint32_t m_inacTimerSec;
            
            SLEEPERROR doSleepNowEx(bool fromTimer);
            
        };
    }
}


#endif /* ESP32_C3_SLEEP_H */

