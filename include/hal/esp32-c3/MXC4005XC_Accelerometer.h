
#ifndef MXC4005XC_ACCELEROMETER_H
#define MXC4005XC_ACCELEROMETER_H
#include "include/hal/PocuterAccelerometer.h"
#include "include/hal/PocuterI2C.h"
namespace PocuterLib {
    namespace HAL {
        class MXC4005XC_Accelerometer : public PocuterAccelerometer {
        public:
            MXC4005XC_Accelerometer(PocuterI2C* bus);
            
            virtual ~MXC4005XC_Accelerometer();
        private:
            PocuterI2C* m_i2cBus;
        };
    }
}


#endif /* MXC4005XC_ACCELEROMETER_H */

