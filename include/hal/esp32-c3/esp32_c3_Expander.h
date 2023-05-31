
#ifndef ESP32_C3_EXPANDER_H
#define ESP32_C3_EXPANDER_H

#include <stdint.h>
#include <vector>

#include "include/hal/PocuterI2C.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include <driver/gpio.h>

#define EXPANDER_OUT   0
#define EXPANDER_IN    1


namespace PocuterLib {
    namespace HAL {
        class esp32_c3_Expander {
        public:
            
            typedef void (expanderEventHandler)(uint8_t, uint8_t, void*);
            void registerEventHandler(expanderEventHandler*, void*);
            void unregisterEventHandler(expanderEventHandler* e);
            
            void registerI2Cbus(PocuterI2C*);
            
            static esp32_c3_Expander* Instance(void);
            uint8_t pinMode(uint8_t port, uint8_t pin, uint8_t pinDirection);
            uint8_t ledMode(uint8_t port, uint8_t pin, uint8_t ledMode);
            uint8_t setPin(uint8_t port, uint8_t pin, uint8_t value);
            uint8_t setLed(uint8_t port, uint8_t pin, uint8_t value);
            
            uint8_t readPin(uint8_t port, uint8_t pin);
            uint8_t readPort(uint8_t port);
            
            void registerInterruptPin(uint8_t port, uint8_t pin);
            
            void pauseInterruptHandler();
            void resumeInterruptHandler();
        private:
            static QueueHandle_t m_InterruptQueue;
            static esp32_c3_Expander* m_pInstance;
            static void interruptHandler(void*);
            static void intTask(void *arg);
            esp32_c3_Expander();
            virtual ~esp32_c3_Expander();
            
            PocuterI2C* m_i2c;

            uint8_t m_P0_Dir; 
            uint8_t m_P1_Dir; 
            
            uint8_t m_P0_Int; 
            uint8_t m_P1_Int; 
            
            
            uint8_t m_P0_Led; 
            uint8_t m_P1_Led; 
            
            uint8_t m_P0_Val; 
            uint8_t m_P1_Val; 
            
            bool m_expanderOnline;
            struct eventHandlerStruct{
                expanderEventHandler* function;
                void* userData;
            };
            std::vector<eventHandlerStruct> m_eventHandler;
            SemaphoreHandle_t m_eventHandlerSemaphore;
            
            gpio_num_t m_interruptPin;

        };
    }
}

#endif /* ESP32_C3_EXPANDER_H */

