
#include <stdint.h>

#ifndef POCUTER_RGBLED_H
#define POCUTER_RGBLED_H


/**
* @brief Pocuter status LED
*  
*/

class RGBled {
    public:
        enum LEDERROR {
            LEDERROR_OK,
            LEDERROR_FAILURE
            
        };
        /**
        * @brief Setting Status LED
        *
        * @param ledNumber the led id
        * @param r value
        * @param g value
        * @param b value
        *
        * @return
        *     - LEDERROR_OK Success
        *     - LEDERROR_FAILURE failed to set RGB
        */
        virtual LEDERROR setRGB(uint8_t ledNumber, uint8_t r, uint8_t g, uint8_t b) = 0;
        
    private:
};
    


#endif /* POCUTER_RGBLED_H */

