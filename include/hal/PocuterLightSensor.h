#ifndef POCUTERLIGHTSENSOR_H
#define POCUTERLIGHTSENSOR_H
#include <stdint.h>

/**
* @brief options to access the Pocuter light sensor
*  
*/
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
        /**
        * @brief register an event callback
        * 
        * @note the event handler is called every time a limit has been under or exceeded
        * 
        * @param h a pointer to the callback function
        * @param limit the limit to call the eventhandler
        * @param tollerance a tollerance for debouncing event handler requests.
        * @param u a pointer to user data
        * 
        * @return LIGHTERROR_OK
        * 
        */
        virtual LIGHTERROR registerEventHandler(lightEventHandler* h, uint16_t limit, uint16_t tollerance, void* u) = 0;
        
        
        /**
        * @brief unregister the event handler
        * 
        */
        virtual void unregisterEventHandler() = 0;
        
         /**
        * @brief poll the light value
        * @return current light value
        */
        virtual uint16_t getValue() = 0;
        
    private:
        
};


#endif /* POCUTERLIGHTSENSOR_H */

