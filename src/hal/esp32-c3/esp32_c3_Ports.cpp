#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_PORTS
#include "include/hal/esp32-c3/esp32_c3_Ports.h"
#include "include/hal/esp32-c3/esp32_c3_Expander.h"
#include "include/hal/esp32-c3/esp32_c3_ADC.h"

using namespace PocuterLib::HAL;

QueueHandle_t esp32_c3_Ports::m_InterruptQueue;

const esp32_c3_Ports::PORT_WIRE esp32_c3_Ports::s_ports[6] = {
    {
                true,
                false,
                true,
                0,
                4,
                4
                
    },
    {
                false,
                true,
                false,
                1,
                1,
                0
                
    },
    {
                false,
                true,
                false,
                1,
                0,
                0
                
    },
    {
                false,
                true,
                false,
                1,
                7,
                0
                
    },
    {
                false,
                true,
                false,
                1,
                6,
                0
                
    },
    {
                true,
                false,
                true,
                0,
                3,
                3
                
    }
};


esp32_c3_Ports::esp32_c3_Ports() {
    for (int i = 0; i <= PORT_5; i++) {
        m_portConfigurations[i].isInitialized = false;
        m_portConfigurations[i].event = NULL;
    }
    m_expanderEventHandlerRegistered = false;
    m_nativePortsTaskStarted = false;
    
    m_InterruptQueue = xQueueCreate(16, sizeof(uint32_t));
    if(!m_InterruptQueue) abort();
    
}



esp32_c3_Ports::~esp32_c3_Ports() {
}

PocuterPorts::PORTSERROR esp32_c3_Ports::initPort(PORT_NUMBER n, PORT_DIRECTION d, PORT_MODE m, PORT_PULLUP p) {
    if (n > PORT_5) return PORTSERROR_NOT_SUPPORTED;
    
    PORT_WIRE port = s_ports[n];
    
    if (! port.isADC && m == PORT_MODE_ADC) return PORTSERROR_NOT_SUPPORTED;
    if (! port.canPullUp && p == PORT_PULLUP_ON) return PORTSERROR_NOT_SUPPORTED;
    if (m == PORT_MODE_ADC && d != PORT_DIRECTION_IN) return PORTSERROR_NOT_SUPPORTED;
     
    m_portConfigurations[n].m = m;
    m_portConfigurations[n].d = d;
    if (m == PORT_MODE_BINARY) {
        if (port.isExpander) {
            esp32_c3_Expander* expander = esp32_c3_Expander::Instance();
            if (d == PORT_DIRECTION_IN) {
                expander->pinMode(port.port, port.pin, EXPANDER_IN);
            } else {
                expander->pinMode(port.port, port.pin, EXPANDER_OUT);
            }
        } else {

           gpio_config_t io_conf = {};
           io_conf.intr_type = GPIO_INTR_DISABLE;
           if (d == PORT_DIRECTION_IN) {
                io_conf.mode = GPIO_MODE_INPUT;
                io_conf.pin_bit_mask = (1ULL<<port.pin);
                io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;

                if (p == PORT_PULLUP_OFF) {
                    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
                } else {
                    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
                }



                gpio_config(&io_conf);
           } else {
                io_conf.mode = GPIO_MODE_OUTPUT;
                io_conf.pin_bit_mask = (1ULL<<port.pin);
                io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
                io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
                gpio_config(&io_conf);
           }
        }
        m_portConfigurations[n].isInitialized = true;
        return PORTSERROR_OK;
    }
    
    if (m == PORT_MODE_ADC) {
        m_portConfigurations[n].isInitialized = true;
        return PORTSERROR_OK;
    }
    
    return PORTSERROR_NOT_SUPPORTED;
    
}

PocuterPorts::PORTSERROR esp32_c3_Ports::getValue(PORT_NUMBER n, bool* value) {
    if (n > PORT_5) return PORTSERROR_NOT_SUPPORTED;
    if (! m_portConfigurations[n].isInitialized) return PORTSERROR_NOT_INITIALIZED;
    if (m_portConfigurations[n].d != PORT_DIRECTION_IN) return PORTSERROR_NOT_SUPPORTED;
    if (m_portConfigurations[n].m != PORT_MODE_BINARY) return PORTSERROR_NOT_SUPPORTED;
    
    if (s_ports[n].isExpander) {
        esp32_c3_Expander* expander = esp32_c3_Expander::Instance();
        if (expander->readPin(s_ports[n].port, s_ports[n].pin)) *value = true; else *value = false;
        return PORTSERROR_OK;
    } else {
        *value = gpio_get_level((gpio_num_t)s_ports[n].pin);
        return PORTSERROR_OK;
    }
    return PORTSERROR_NOT_SUPPORTED;
}
PocuterPorts::PORTSERROR esp32_c3_Ports::setValue(PORT_NUMBER n, bool value) {
    if (n > PORT_5) return PORTSERROR_NOT_SUPPORTED;
    if (! m_portConfigurations[n].isInitialized) return PORTSERROR_NOT_INITIALIZED;
    if (m_portConfigurations[n].d != PORT_DIRECTION_OUT) return PORTSERROR_NOT_SUPPORTED;
    if (m_portConfigurations[n].m != PORT_MODE_BINARY) return PORTSERROR_NOT_SUPPORTED;
    
    if (s_ports[n].isExpander) {
        esp32_c3_Expander* expander = esp32_c3_Expander::Instance();
        if (! expander->setPin(s_ports[n].port, s_ports[n].pin, value)) return PORTSERROR_UNKNOWN;
        return PORTSERROR_OK;
    } else {
        gpio_set_level((gpio_num_t)s_ports[n].pin, value);
        return PORTSERROR_OK;
    }
    return PORTSERROR_NOT_SUPPORTED;
}



PocuterPorts::PORTSERROR esp32_c3_Ports::deinitPort(PORT_NUMBER n){
    if (n > PORT_5) return PORTSERROR_NOT_SUPPORTED;
    if (! m_portConfigurations[n].isInitialized) return PORTSERROR_NOT_INITIALIZED;
    m_portConfigurations[n].isInitialized = false;
    if (m_portConfigurations[n].event) {
        m_portConfigurations[n].event = NULL;
    }
    
    return PORTSERROR_OK;
}



PocuterPorts::PORTSERROR esp32_c3_Ports::getValue(PORT_NUMBER n, uint16_t* value){
    if (n > PORT_5) return PORTSERROR_NOT_SUPPORTED;
    if (! m_portConfigurations[n].isInitialized) return PORTSERROR_NOT_INITIALIZED;
    if (m_portConfigurations[n].d != PORT_DIRECTION_IN) return PORTSERROR_NOT_SUPPORTED;
    if (m_portConfigurations[n].m != PORT_MODE_ADC) return PORTSERROR_NOT_SUPPORTED;
    *value = esp32_c3_ADC::singleRead((adc1_channel_t)s_ports[n].ADCchannel);
    
    return PORTSERROR_OK;
    
}



void IRAM_ATTR esp32_c3_Ports::interruptHandler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(m_InterruptQueue, &gpio_num, NULL);
}
void esp32_c3_Ports::intTask(void *arg)
{
    esp32_c3_Ports* myself = (esp32_c3_Ports*)arg;
    uint32_t io_num;
    while(1)
      {
        if(xQueueReceive(m_InterruptQueue, &io_num, portMAX_DELAY) == pdTRUE )
        {
           for (int i = 0; i <= PORT_5; i++) {
               PORT_WIRE port = s_ports[i];
               if (port.pin == io_num && ! port.isExpander) {
                   PORT_CONFIG* c = &myself->m_portConfigurations[i];
                   if (c->event) {
                       uint16_t v = gpio_get_level((gpio_num_t)s_ports[i].pin);
                       if (c->lastState != v) {
                           c->lastState = v;
                           c->event((PORT_NUMBER)i, v, c->userData);
                       }
                       
                   }
                   break; // we did find the gpio and can stop here
               }
           }
        }
      
     
    }

    
   vTaskDelete(NULL);
}
void esp32_c3_Ports::expEventHandler(uint8_t a, uint8_t b, void* m) {
    esp32_c3_Ports* myself = (esp32_c3_Ports*)m;
    for (int i = 0; i <= PORT_5; i++) {
        PORT_WIRE port = s_ports[i];
        if (port.isExpander) {
            PORT_CONFIG* c = &myself->m_portConfigurations[i];
            if (c->event) {
                uint8_t v;
                if (port.port) v = b & (1 << port.pin); else v = a & (1 << port.pin); 
                if (c->lastState != v) {
                    c->lastState = v;
                    c->event((PORT_NUMBER)i, (v != 0), c->userData);
                }
            }
        }
    }
}
PocuterPorts::PORTSERROR esp32_c3_Ports::registerEventHandler(PORT_NUMBER n, portEventHandler* h, void* u){
    if (n > PORT_5) return PORTSERROR_NOT_SUPPORTED;
    if (! m_portConfigurations[n].isInitialized) return PORTSERROR_NOT_INITIALIZED;
    if (m_portConfigurations[n].d != PORT_DIRECTION_IN) return PORTSERROR_NOT_SUPPORTED;
    m_portConfigurations[n].event = h;
    m_portConfigurations[n].userData = u;
    getValue(n, &m_portConfigurations[n].lastState);
    if (s_ports[n].isExpander) {
        esp32_c3_Expander* expander = esp32_c3_Expander::Instance();
        expander->registerInterruptPin(s_ports[n].port, s_ports[n].pin);
        if (! m_expanderEventHandlerRegistered) {
            expander->registerEventHandler(esp32_c3_Ports::expEventHandler, this);
            m_expanderEventHandlerRegistered = true;
        }
        return PORTSERROR_OK;
    } else {
        if (! m_nativePortsTaskStarted) {
            m_nativePortsTaskStarted = true;
            xTaskCreate(&intTask, "intTaskPorts", 4000, this, 10, NULL);
        }
        m_portConfigurations[n].lastState = gpio_get_level((gpio_num_t)s_ports[n].pin);
        gpio_set_intr_type((gpio_num_t)s_ports[n].pin, GPIO_INTR_ANYEDGE);
        uint32_t pin = s_ports[n].pin;
        gpio_isr_handler_add((gpio_num_t)s_ports[n].pin, &esp32_c3_Ports::interruptHandler, (void*)pin);
        return PORTSERROR_OK;
    }
    
    // this point should never be reached
    return PORTSERROR_NOT_SUPPORTED;
}
            
            
#endif
