
#ifndef MXC4005XC_ACCELEROMETER_H
#define MXC4005XC_ACCELEROMETER_H
#include "include/hal/PocuterAccelerometer.h"
#include "include/hal/PocuterI2C.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
namespace PocuterLib {
    namespace HAL {
        class MXC4005XC_Accelerometer : public PocuterAccelerometer {
        public:
           
            
            MXC4005XC_Accelerometer(PocuterI2C* bus);
            
            ACCERROR registerEventHandler(accEventHandler*, void*);
            void unregisterEventHandler(); 
       
            ACCERROR isOnline();
            ACCERROR getState(State*);
            ACCERROR setScale(ACCSCALE);
            ACCERROR getTemperature(int8_t*);
            virtual ~MXC4005XC_Accelerometer();
        private:
            static void interruptHandler(void*);
            static void intTask(void *arg);
            
            static QueueHandle_t m_InterruptQueue;
            PocuterI2C* m_i2cBus;
            bool m_online;
            uint8_t m_controlRegister;
            int m_deviceID;
            
            accEventHandler* m_accEventHandler;
            void* m_accEventHandlerUserData;
        };
    }
}


#endif /* MXC4005XC_ACCELEROMETER_H */

