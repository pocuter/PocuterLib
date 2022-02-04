
#ifndef RGBLED_H
#define RGBLED_H

#include <stdint.h>
#include "include/hal/RGBled.h"
#include "esp32_c3_Expander.h"


namespace PocuterLib {
    namespace HAL {
        class esp32_c3_RGBled : public RGBled {
            public:
                esp32_c3_RGBled();
                ~esp32_c3_RGBled();
                int setRGB(uint8_t ledNumber, uint8_t r, uint8_t g, uint8_t b);
            private:
                esp32_c3_Expander* m_expander;  
        };
    }
}


#endif /* RGBLED_H */

