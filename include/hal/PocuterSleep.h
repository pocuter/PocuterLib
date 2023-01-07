
#ifndef POCUTERSLEEP_H
#define POCUTERSLEEP_H
#include <stddef.h>
/**
* @brief Pocuter Sleep settings
*  
*/
class PocuterSleep {
    public:
        enum SLEEPERROR {
            SLEEPERROR_OK,                          /*!< everything went well */
            SLEEPERROR_MODE_TIMER_NOT_SET,          /*!< could not set the timer */
            SLEEPERROR_MODE_NOT_POSSIBLE,           /*!< cannot set this sleep mode */
            SLEEPERROR_COULD_NOT_SET_TIMER_WAKEUP,  /*!< cannot set the sleep timer */
            SLEEPERROR_COULD_NOT_SET_GPIO_WAKEUP,   /*!< cannot set the gpios for wake up */
            SLEEPERROR_COULD_NOT_SET_WLAN_WAKEUP,   /*!< cannot set WLAN for wake up */
            SLEEPERROR_SLEEP_INTERRUPTED,           /*!< going to sleep was interrupted by the eventhandler */
            SLEEPERROR_UNKNOWN

        };
        enum SLEEPTIMER_INTERRUPT {
            SLEEPTIMER_INTERRUPT_BY_BUTTON = 0x01,  /*!< interrupt sleep timer by a button */
            SLEEPTIMER_INTERRUPT_BY_SHAKE = 0x02    /*!< interrupt sleep timer by shaking */
        };
        enum SLEEP_MODE{
            SLEEP_MODE_LIGHT, /*!< light sleep mode needs more power but the program can be continued after waking up   */
            SLEEP_MODE_DEEP,  /*!< The deep sleep mode needs less power, but the program restarts after waking up. Only a maximum of 8kb of the RTC memory declared with "DEEP_SLEEP_MEMORY" is kept.  */
        };
        enum WAKEUP_MODE{
            WAKEUP_MODE_USERPORT_0_LOW = 0x01, /*!< available in deep sleep and light sleep   */
            WAKEUP_MODE_USERPORT_5_LOW = 0x02, /*!< available in deep sleep and light sleep   */
            WAKEUP_MODE_USERPORT_0_HIGH = 0x04,  /*!< available in deep sleep and light sleep   */
            WAKEUP_MODE_USERPORT_5_HIGH = 0x08,  /*!< available in deep sleep and light sleep   */
            WAKEUP_MODE_ANY_BUTTON = 0x10,  /*!< available in light sleep only  */
            WAKEUP_MODE_SHAKE = 0x20,  /*!< available in light sleep only  */
            WAKEUP_MODE_TIMER = 0x40, /*!< available in deep sleep and light sleep   */
            WAKEUP_MODE_WIFI = 0x80   /*!< available in light sleep only  */
        };
        enum WAKEUP_CAUSE{
            WAKEUP_CAUSE_POWER_ON,  /*!< waked up by powering the device  */
            WAKEUP_CAUSE_GPIO,      /*!< waked up by gpio (als0 a button is possible)  */
            WAKEUP_CAUSE_TIMER,     /*!< waked up by timer  */
            WAKEUP_CAUSE_WIFI       /*!< waked up by WIFI  */
        };
        enum SLEEP_EVENT{
            SLEEP_EVENT_ENTER_SLEEP,
            SLEEP_EVENT_WAKEUP
        };
        
        typedef int WAKEUP_MODES;
        typedef int SLEEPTIMER_INTERRUPTS;
        typedef bool (sleepEventHandler)(SLEEP_EVENT, void*);  /*!< type definition of the event callback function. If you return false, going to sleep or wake up will be interrupted */
        
        
         /**
        * @brief register an event callback
        * 
        * @note the event handler is called every time the device is going to sleep or wakes up
        * 
        * @param e a pointer to the callback function
        * @param u a pointer to user data sent to the event handler on each event
        * 
        * 
        */
        virtual void registerEventHandler(sleepEventHandler* e, void* u) = 0;
        
        /**
        * @brief unregister the event handler
        * 
        */
        virtual void unregisterEventHandler() = 0;
        
        /**
        * @brief net the sleep mode
        * 
        * @note depending on the sleep mode selected, only certain wake up modes are available
        * 
        * @param sm the choosen SLEEP_MODE
        * 
        * @return
        *     - SLEEPERROR_OK or an error
        * 
        * 
        */
        virtual SLEEPERROR setSleepMode(SLEEP_MODE sm) = 0;
        
        /**
        * @brief set the wake up mode
        * 
        * @note depending on the sleep mode selected, only certain wake up modes are available
        * 
        * @param wm here you can choose multiple modes linked with an or. (e.g. WAKEUP_MODE_ANY_BUTTON | WAKEUP_MODE_SHAKE)
        * 
        * @return
        *     - SLEEPERROR_OK 
        *     - SLEEPERROR_MODE_NOT_POSSIBLE in the chosen sleep mode there is one or wake up option not available
        * 
        */
        virtual SLEEPERROR setWakeUpModes(WAKEUP_MODES wm) = 0;
        
        
         /**
        * @brief set the wake up timer
        * 
        * @note set a wake up timer in seconds. You have also to set the WAKEUP_MODE_TIMER with setWakeUpModes.
        * 
        * @param sec max sleep seconds
        * 
        * @return
        *     - SLEEPERROR_OK 
        * 
        */
        virtual SLEEPERROR setWakeUpTimer(uint32_t sec) = 0;
        
        
        /**
        * @brief sleep now
        * 
        * @note the device will go to sleep, immediately
        * 
        * @return
        *     - SLEEPERROR_OK 
        *     - SLEEPERROR_COULD_NOT_SET_GPIO_WAKEUP could not set the wake up gpios
        *     - SLEEPERROR_COULD_NOT_SET_TIMER_WAKEUP could not set the wake up timer
        *     - SLEEPERROR_COULD_NOT_SET_WLAN_WAKEUP could not set WLAN wake up
        * 
        */
        virtual SLEEPERROR doSleepNow() = 0;
        
        
        /**
        * @brief set the behavior of the device
        * 
        * @param sec wait sec before go to sleep. 0 will never sleep.
        * @param cause wait could cause a sleep timer interrupt
        * @param saveTimeout save the sec value as default. It will restore the default after reboot (saved on SD)
        * 
        * @return
        *     - SLEEPERROR_OK 
        * 
        */
        virtual SLEEPERROR setInactivitySleep(uint32_t sec, SLEEPTIMER_INTERRUPTS cause, bool saveTimeout = false) = 0;
        
        /**
        * @brief disable sleep timer
        * 
        * @note internally call setInactivitySleep with a timeout of 0 and all interrupt flags
        * @note there is a race condition between the initialization of the Pocuter object and the creation of the sleep timer. There needs to be a suitable delay ~50ms between creating the Pocuter object and calling pocuter->Sleep->disable()
        * 
        * @return
        *     - SLEEPERROR_OK 
        * 
        */
        SLEEPERROR disable() { return this->setInactivitySleep( 0, (SLEEPTIMER_INTERRUPTS) 0x03 ); }

         /**
        * @brief why did the device waked up?
        * @note wake up by WAKEUP_CAUSE_GPIO could also mean by a button.
        * @return
        *     - WAKEUP_CAUSE 
        * 
        */
        virtual WAKEUP_CAUSE getWakeUpCause() = 0;
        
        
        
        
};


#endif /* POCUTERSLEEP_H */

