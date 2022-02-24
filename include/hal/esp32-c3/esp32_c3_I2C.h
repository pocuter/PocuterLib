

#ifndef ESP32_C3_I2C_H
#define ESP32_C3_I2C_H
#include "driver/i2c.h"
#include "include/hal/PocuterI2C.h"

namespace PocuterLib {
    namespace HAL {
        class esp32_c3_I2C : public PocuterI2C{
        public:
            esp32_c3_I2C(int port);
            esp32_c3_I2C(const esp32_c3_I2C& orig);
            virtual ~esp32_c3_I2C();
            int write(uint8_t i2c_addr, uint8_t i2c_reg, uint8_t* data_wr, size_t size);
            int write(uint8_t i2c_addr, uint8_t i2c_reg, uint8_t data_wr);
            
            int read(uint8_t i2c_addr, uint8_t i2c_reg, uint8_t* data_rd, size_t size);
            int read(uint8_t i2c_addr, uint8_t i2c_reg);
        private:
            static SemaphoreHandle_t m_i2cSemaphore;
            bool m_driverInstalled;
            int m_port;
            
        };
    }
}

#endif /* ESP32_C3_I2C_H */

