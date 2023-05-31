
#ifndef POCUTERDEVICETYPE_H
#define POCUTERDEVICETYPE_H

class PocuterDeviceType {
    public:
        enum DEVICE_TYPES {
            DEVICE_TYPE_UNKNOWN = 0,
            DEVICE_TYPE_POCUTER_1,
            DEVICE_TYPE_POCKETSTAR_2

        };
        /**
        * @brief get the current device type
        * 
        * DEVICE_TYPES
        * 
        */       
        static DEVICE_TYPES deviceType;
        
};

#endif /* POCUTERDEVICETYPE_H */

