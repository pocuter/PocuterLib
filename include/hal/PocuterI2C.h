#include <stdint.h>
#include <stddef.h>
#ifndef POCUTERI2C_H
#define POCUTERI2C_H
class PocuterI2C {
    public:
        virtual int write(uint8_t i2c_addr, uint8_t i2c_reg, uint8_t* data_wr, size_t size) = 0;
        virtual int write(uint8_t i2c_addr, uint8_t i2c_reg, uint8_t data_wr) = 0;
            
        virtual int read(uint8_t i2c_addr, uint8_t i2c_reg, uint8_t* data_rd, size_t size) = 0;
        virtual int read(uint8_t i2c_addr, uint8_t i2c_reg) = 0;
    private:

};


#endif /* POCUTERI2C_H */

