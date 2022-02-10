
#include "include/hal/esp32-c3/esp32_c3_Expander.h"
#include "include/hal/esp32-c3/Pocuter_Pins.h"

#include <unistd.h>


using namespace PocuterLib::HAL;

esp32_c3_Expander* esp32_c3_Expander::m_pInstance = NULL;
QueueHandle_t esp32_c3_Expander::m_InterruptQueue;


#define CONFIG_P0             0x04
#define CONFIG_P1             0x05
#define OUTPUT_P0             0x02
#define LED_P0                0x12
#define LED_P1                0x13
#define OUTPUT_P1             0x03 
#define INPUT_P0              0x00
#define INPUT_P1              0x01
#define EXPANDER_ID           0x10

#define EXPANDER_INT_PIN   GPIO_NUM_20 


esp32_c3_Expander* esp32_c3_Expander::Instance() {
    if (!m_pInstance) {
        m_pInstance = new esp32_c3_Expander();
    }
    return m_pInstance;
}
void esp32_c3_Expander::registerEventHandler(expanderEventHandler* e, void* d) {
    xSemaphoreTake(m_eventHandlerSemaphore, portMAX_DELAY);
    eventHandlerStruct s;
    s.function = e;
    s.userData = d;
    m_eventHandler.push_back(s);
    xSemaphoreGive(m_eventHandlerSemaphore);
}
void esp32_c3_Expander::unregisterEventHandler(expanderEventHandler* e) {
    xSemaphoreTake(m_eventHandlerSemaphore, portMAX_DELAY);
    for (std::vector<eventHandlerStruct>::iterator it = m_eventHandler.begin() ; it != m_eventHandler.end(); ++it) {
        if ((*it).function == e) {
            m_eventHandler.erase(it);
            break;
        }
    }
    xSemaphoreGive(m_eventHandlerSemaphore);
}

void IRAM_ATTR esp32_c3_Expander::interruptHandler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(m_InterruptQueue, &gpio_num, NULL);
}
void esp32_c3_Expander::intTask(void *arg)
{
    esp32_c3_Expander* myself = (esp32_c3_Expander*)arg;
    uint32_t io_num;
    while(1)
      {
        if(xQueueReceive(m_InterruptQueue, &io_num, portMAX_DELAY) == pdTRUE )
        {
            uint8_t port1 = myself->readPort(0);
            uint8_t port2 = myself->readPort(1);
            
            xSemaphoreTake(myself->m_eventHandlerSemaphore, portMAX_DELAY);
            for (std::vector<eventHandlerStruct>::iterator it = myself->m_eventHandler.begin() ; it != myself->m_eventHandler.end(); ++it) {
                ((*it).function)(port1, port2, (*it).userData);
            }
            xSemaphoreGive(myself->m_eventHandlerSemaphore);
        }
      
     
    }

    
   vTaskDelete(NULL);
}

esp32_c3_Expander::esp32_c3_Expander() {
    m_expanderOnline = false;
    
    
    m_InterruptQueue = xQueueCreate(16, sizeof(uint32_t));
    if(!m_InterruptQueue) abort();
    
    m_eventHandlerSemaphore = xSemaphoreCreateBinary();
    if (!m_eventHandlerSemaphore) abort();
    xSemaphoreGive(m_eventHandlerSemaphore);
    
   
    m_i2c = new esp32_c3_I2C(0);
    m_P0_Dir = 0xFF;
    m_P1_Dir = 0xFF;
    
    m_P0_Int = 0xFF;
    m_P1_Int = 0xFF;
    
    m_P0_Led = 0xFF;
    m_P1_Led = 0xFF;
        
    int id = m_i2c->read(EXPANDER_I2C_ADDRESS, EXPANDER_ID);
    if (id == 0x23) {
        m_expanderOnline = true;
    }
     
    
    m_i2c->write(EXPANDER_I2C_ADDRESS, EXP_REG_GLOBAL_CONTROL, 0x13);
    m_i2c->write(EXPANDER_I2C_ADDRESS, EXP_REG_CONFIG_P0, m_P0_Dir);
    m_i2c->write(EXPANDER_I2C_ADDRESS, EXP_REG_CONFIG_P1, m_P1_Dir);
    //m_i2c->write(EXPANDER_I2C_ADDRESS, EXP_REG_INT_CONFIG_P0, ~(1 << 1));
    
    
    m_i2c->write(EXPANDER_I2C_ADDRESS, EXP_REG_INT_CONFIG_P0, m_P0_Int);
    m_i2c->write(EXPANDER_I2C_ADDRESS, EXP_REG_INT_CONFIG_P1, m_P1_Int);
    
    
    xTaskCreate(&intTask, "intTask", 4000, this, 10, NULL);
    
    gpio_set_intr_type(EXPANDER_INT_PIN, GPIO_INTR_NEGEDGE);
    
    gpio_install_isr_service(0);
    gpio_isr_handler_add(EXPANDER_INT_PIN, &interruptHandler, (void*)EXPANDER_INT_PIN);
   
    
    xQueueSendFromISR(m_InterruptQueue, &m_P0_Int, NULL);
    
}


esp32_c3_Expander::~esp32_c3_Expander() {
    delete m_i2c;
}

void esp32_c3_Expander::registerInterruptPin(uint8_t port, uint8_t pin) {
    
    if (port == 0) {
        m_P0_Int &= ~(1 << pin);
        m_i2c->write(EXPANDER_I2C_ADDRESS, EXP_REG_INT_CONFIG_P0, m_P0_Int);
    }else {
        m_P1_Int &= ~(1 << pin);
        m_i2c->write(EXPANDER_I2C_ADDRESS, EXP_REG_INT_CONFIG_P1, m_P1_Int);
    }
}


uint8_t esp32_c3_Expander::setLed(uint8_t port, uint8_t pin, uint8_t value)
{
    if (! m_expanderOnline) return 0;
    if (port > 1 || pin > 8) return 0;
    uint8_t reg;
    if (port == 0){
        reg = 0x24 + pin;
    } else {
        reg = 0x20 + pin;
        if (reg > 0x23) reg += 8;
    }
    
    m_i2c->write(EXPANDER_I2C_ADDRESS, reg, value);
    return 1;
}
uint8_t esp32_c3_Expander::readPort(uint8_t port)
{
  if (! m_expanderOnline) return 0;
  uint8_t portReg;
 
  if(port){
    portReg = INPUT_P1;
  }
  else{
    portReg = INPUT_P0;
  }
  int v = m_i2c->read(EXPANDER_I2C_ADDRESS, portReg);
  if (v < 0) return 0;
  return v;
}
uint8_t esp32_c3_Expander::readPin(uint8_t port, uint8_t pin)
{
  
  return readPort(port) & (1 << pin);
  
}
uint8_t esp32_c3_Expander::setPin(uint8_t port, uint8_t pin, uint8_t value)
{
  if (! m_expanderOnline) return 0;
  uint8_t portVal;
  uint8_t portReg;
 
  if(port){
    if(value)
      m_P1_Val |= (1 << pin);
    else  
      m_P1_Val &= ~(1 << pin);
      
    portVal = m_P1_Val;
    portReg = OUTPUT_P1;
    }
  else{
    if(value)
      m_P0_Val |= (1 << pin);
    else  
      m_P0_Val &= ~(1 << pin);
      
    portVal = m_P0_Val; 
    portReg = OUTPUT_P0;
    }
  m_i2c->write(EXPANDER_I2C_ADDRESS, portReg, portVal); // EXPANDER_I2C_ADDRESS
  return 1;
 
  
}

uint8_t esp32_c3_Expander::pinMode(uint8_t port, uint8_t pin, uint8_t pinDirection)
{
  if (! m_expanderOnline) return 0;
  uint8_t portNr = port;
  uint8_t portReg, portDir;

  if(pinDirection == EXPANDER_OUT)
  {
    if(portNr)
    {
      m_P1_Dir &= ~(1 << pin); 
      portDir = m_P1_Dir;
      portReg = CONFIG_P1;
    }
    else
    {
      m_P0_Dir &= ~(1 << pin); 
      portDir = m_P0_Dir;
      portReg = CONFIG_P0;
    }
  }
  else if(pinDirection == EXPANDER_IN)
  {
    if(portNr)
    {
      m_P1_Dir |= (1 << pin);
      portDir = m_P1_Dir;
      portReg = CONFIG_P1;
    }
    else
    {
      m_P0_Dir |= (1 << pin);
      portDir = m_P0_Dir;
      portReg = CONFIG_P0;
    }
  }
  m_i2c->write(EXPANDER_I2C_ADDRESS, portReg, portDir); // EXPANDER_I2C_ADDRESS
  return 1;
}

uint8_t esp32_c3_Expander::ledMode(uint8_t port, uint8_t pin, uint8_t ledMode)
{
  if (! m_expanderOnline) return 0;
  uint8_t portNr = port;
  uint8_t portReg, portLed;

  if(ledMode == EXPANDER_OUT)
  {
    if(portNr)
    {
      m_P1_Led &= ~(1 << pin); 
      portLed = m_P1_Led;
      portReg = LED_P1;
    }
    else
    {
      m_P0_Led &= ~(1 << pin); 
      portLed = m_P0_Led;
      portReg = LED_P0;
    }
  }
  else if(ledMode == EXPANDER_IN)
  {
    if(portNr)
    {
      m_P1_Led |= (1 << pin);
      portLed = m_P1_Led;
      portReg = LED_P1;
    }
    else
    {
      m_P0_Led |= (1 << pin);
      portLed = m_P0_Led;
      portReg = LED_P0;
    }
  }
  
  m_i2c->write(EXPANDER_I2C_ADDRESS, portReg, portLed); // EXPANDER_I2C_ADDRESS
  return 1;
}
