
#include <stdint.h>

#ifndef POCUTER_RGBLED_H
#define POCUTER_RGBLED_H

class RGBled {
    public:
        virtual int setRGB(uint8_t ledNumber, uint8_t r, uint8_t g, uint8_t b) = 0;
        
    private:
};
    


#endif /* POCUTER_RGBLED_H */

