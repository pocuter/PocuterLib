#include "include/PocuterLibConfig.h"

#ifndef POCUTER_DISABLE_ACC 
#include <stdint.h>
#include "include/hal/esp32-c3/MXC4005XC_Accelerometer.h"
#include "driver/gpio.h"

using namespace PocuterLib::HAL;

QueueHandle_t MXC4005XC_Accelerometer::m_InterruptQueue;

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define CHECK_BIT(x, pos) (x & (1UL << pos) )


#define I2C_ACC_ADDR  0x15
#define REGISTER_SHAKE_INT              0x00
#define REGISTER_DATA_READY_INT         0x01
#define REGISTER_SHAKE_CLEAR            0x00
#define REGISTER_DATA_READY_CLEAR       0x01
#define REGISTER_STATUS                 0x02
#define REGISTER_X_AXIS_MSB             0x03
#define REGISTER_X_AXIS_LSB             0x04
#define REGISTER_Y_AXIS_MSB             0x05
#define REGISTER_Y_AXIS_LSB             0x06
#define REGISTER_Z_AXIS_MSB             0x07
#define REGISTER_Z_AXIS_LSB             0x08
#define REGISTER_TEMPERATURE            0x09
#define REGISTER_SHAKE_MASK             0x0A
#define REGISTER_DATA_READY_MASK        0x0B
#define REGISTER_SHAKE_PARAMETERS       0x0C
#define REGISTER_OPERATING_CONTROL      0x0D
#define REGISTER_DEVICE_ID              0x0E
#define REGISTER_WHO_AM_I               0x0F

#define ACC_INT_PIN   GPIO_NUM_7

MXC4005XC_Accelerometer::MXC4005XC_Accelerometer(PocuterI2C* bus) {
    m_i2cBus = bus;
    m_accEventHandler = NULL;
    m_i2cBus->write(I2C_ACC_ADDR, REGISTER_DATA_READY_MASK, 0x10); // soft - reset the chip
    
    m_online = false;
    m_deviceID = -1;
    uint8_t trys = 5;
    do {
        vTaskDelay(configTICK_RATE_HZ / 500);
        m_deviceID = m_i2cBus->read(I2C_ACC_ADDR, REGISTER_DEVICE_ID);
    } while ((m_deviceID == -1 || m_deviceID > 0x0F)&& --trys > 0);
    
    if (trys) {
        m_online = true;
    } else {
        return;
    }
    m_controlRegister = m_i2cBus->read(I2C_ACC_ADDR, REGISTER_OPERATING_CONTROL);
    
    m_InterruptQueue = xQueueCreate(16, sizeof(uint32_t));
    if(!m_InterruptQueue) abort();
    
    xTaskCreate(&intTask, "intTaskACC", 4000, this, 10, NULL);
    
    gpio_set_intr_type(ACC_INT_PIN, GPIO_INTR_NEGEDGE);
    
    gpio_install_isr_service(0);
    gpio_isr_handler_add(ACC_INT_PIN, &interruptHandler, (void*)ACC_INT_PIN);
   
    m_i2cBus->write(I2C_ACC_ADDR, REGISTER_SHAKE_MASK, 0xCF);
    m_i2cBus->write(I2C_ACC_ADDR, REGISTER_DATA_READY_MASK, 0x00);
    
   
}
PocuterAccelerometer::ACCERROR MXC4005XC_Accelerometer::getState(State* st) {
    if (! m_online) return ACCERROR_OFFLINE;
    uint8_t buffer[6] = {0};
    if (! m_i2cBus->read(I2C_ACC_ADDR, REGISTER_X_AXIS_MSB, buffer, 6)) return ACCERROR_COMMUNICATION_FAILED;
    st->x = (((buffer[0] << 8) | (buffer[1])) >> 0);
    st->y = (((buffer[2] << 8) | (buffer[3])) >> 0);
    st->z = (((buffer[4] << 8) | (buffer[5])) >> 0);
    
    return ACCERROR_OK;
    
} 
PocuterAccelerometer::ACCERROR MXC4005XC_Accelerometer::isOnline() {
    if(! m_online) return ACCERROR_OFFLINE;
    return ACCERROR_OK;
}
PocuterAccelerometer::ACCERROR MXC4005XC_Accelerometer::setScale(ACCSCALE scale) {
    if (! m_online) return ACCERROR_OFFLINE;
    CLEAR_BIT(m_controlRegister, 4); // to be shure, that this bit is not set (Cksel)
    if (scale == G2) { CLEAR_BIT(m_controlRegister, 6); CLEAR_BIT(m_controlRegister, 5); }
    if (scale == G4) { CLEAR_BIT(m_controlRegister, 6); SET_BIT(m_controlRegister, 5); }
    if (scale == G8) { SET_BIT(m_controlRegister, 6); CLEAR_BIT(m_controlRegister, 5); }
    if (m_i2cBus->write(I2C_ACC_ADDR, REGISTER_OPERATING_CONTROL, m_controlRegister)) return ACCERROR_OK;
    return ACCERROR_COMMUNICATION_FAILED;
}
PocuterAccelerometer::ACCERROR MXC4005XC_Accelerometer::getTemperature(int8_t* t) {
    if (! m_online) return ACCERROR_OFFLINE;
    int tv = m_i2cBus->read(I2C_ACC_ADDR, REGISTER_TEMPERATURE);
    if (tv == -1) return ACCERROR_COMMUNICATION_FAILED;
    *t = (tv &0xFF);
    *t = 25 + *t;
    return ACCERROR_OK;
    
}

void IRAM_ATTR MXC4005XC_Accelerometer::interruptHandler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(m_InterruptQueue, &gpio_num, NULL);
}
void MXC4005XC_Accelerometer::intTask(void *arg)
{
    MXC4005XC_Accelerometer* myself = (MXC4005XC_Accelerometer*)arg;
    uint32_t io_num;
    while(1)
      {
        if(xQueueReceive(m_InterruptQueue, &io_num, portMAX_DELAY) == pdTRUE )
        {
            
            uint8_t a = myself->m_i2cBus->read(I2C_ACC_ADDR, REGISTER_SHAKE_INT);
            if (a & 0x0F) {
                if (myself->m_accEventHandler) myself->m_accEventHandler(ACC_SHAKE, myself->m_accEventHandlerUserData);
            }
            if (a & 0xC0) {
                if (myself->m_accEventHandler) myself->m_accEventHandler(ACC_ORIENTATION, myself->m_accEventHandlerUserData);
            }
            
            myself->m_i2cBus->write(I2C_ACC_ADDR, REGISTER_SHAKE_INT, a);
           
        }
      
     
    }

    
   vTaskDelete(NULL);
}
PocuterAccelerometer::ACCERROR MXC4005XC_Accelerometer::registerEventHandler(PocuterAccelerometer::accEventHandler* e, void* u) {
    if (! m_online) return ACCERROR_OFFLINE;
    m_accEventHandler = e;
    m_accEventHandlerUserData = u;
    return ACCERROR_OK;
}
void MXC4005XC_Accelerometer::unregisterEventHandler() {
    m_accEventHandler = NULL;
}
MXC4005XC_Accelerometer::~MXC4005XC_Accelerometer() {
}
#endif
