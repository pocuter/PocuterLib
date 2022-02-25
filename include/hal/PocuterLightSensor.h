#ifndef POCUTERLIGHTSENSOR_H
#define POCUTERLIGHTSENSOR_H
#include <stdint.h>

class PocuterLightSensor {
    public:
        enum LIGHTERROR {
            LIGHTERROR_OK,
            LIGHTERROR_UNKNOWN
        };
        enum LIGHTEVENT {
            LIGHT_ABOVE_LIMIT,
            LIGHT_UNDER_LIMIT,
            LIGHT_TOLLERANCE_CHANGED
        };   
    
    
        typedef void (lightEventHandler)(LIGHTEVENT, void*);
        virtual LIGHTERROR registerEventHandler(lightEventHandler*, uint16_t limit, uint16_t tollerance, void*) = 0;
        virtual void unregisterEventHandler() = 0;
        virtual uint16_t getValue() = 0;
        
    private:
        
};


#endif /* POCUTERLIGHTSENSOR_H */

