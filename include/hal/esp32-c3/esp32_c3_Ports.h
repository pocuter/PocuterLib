
#ifndef ESP32_C3_PORTS_H
#define ESP32_C3_PORTS_H

#include "include/hal/PocuterPorts.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
namespace PocuterLib  {
    namespace HAL {
        class esp32_c3_Ports : public PocuterPorts {
        public:
            esp32_c3_Ports();
            virtual ~esp32_c3_Ports();
            
            PORTSERROR initPort(PORT_NUMBER, PORT_DIRECTION, PORT_MODE, PORT_PULLUP);
            PORTSERROR deinitPort(PORT_NUMBER);
            
            PORTSERROR getValue(PORT_NUMBER, uint16_t* value);
            
            
            PORTSERROR getValue(PORT_NUMBER, bool* value);
            PORTSERROR setValue(PORT_NUMBER, bool value);
            
            
            PORTSERROR registerEventHandler(PORT_NUMBER, portEventHandler* h, void* u);
            
            
            
        private:
            
            
            struct PORT_WIRE {
                bool isADC;
                bool isExpander;
                bool canPullUp;
                uint8_t port;
                uint8_t pin;
                uint8_t ADCchannel;
            };
            struct PORT_CONFIG {
                bool isInitialized;
                PORT_MODE m;
                PORT_DIRECTION d;
                portEventHandler* event;
                void* userData;
                bool lastState;
            };
            static const PORT_WIRE s_ports[6];
            PORT_CONFIG m_portConfigurations[6];
            
            bool m_expanderEventHandlerRegistered;
            bool m_nativePortsTaskStarted;
            static QueueHandle_t m_InterruptQueue;
            
            static void expEventHandler(uint8_t, uint8_t, void*);
            static void interruptHandler(void*);
            static void intTask(void *arg);
            
            
        };
    }
}

#endif /* ESP32_C3_PORTS_H */

