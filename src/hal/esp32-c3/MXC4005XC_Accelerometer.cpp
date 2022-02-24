
#include <stdint.h>
#include "include/hal/esp32-c3/MXC4005XC_Accelerometer.h"

using namespace PocuterLib::HAL;

#define I2C_WRITE_ADDR 0x15 // 0xD0 | 0001 0101
#define I2C_READ_ADDR  0x15 // 0xD1 | 0001 0101
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


MXC4005XC_Accelerometer::MXC4005XC_Accelerometer(PocuterI2C* bus) {
    m_i2cBus = bus;
    uint8_t whoAmI = m_i2cBus->read(I2C_READ_ADDR, REGISTER_WHO_AM_I);
    
    
}


MXC4005XC_Accelerometer::~MXC4005XC_Accelerometer() {
}

