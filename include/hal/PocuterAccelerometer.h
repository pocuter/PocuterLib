
#ifndef POCUTERACCELEROMETER_H
#define POCUTERACCELEROMETER_H


class PocuterAccelerometer {
    public:
        enum ACCERROR {
           ACCERROR_OK = 0,
           ACCERROR_OFFLINE, 
           ACCERROR_COMMUNICATION_FAILED,
           ACCERROR_UNKNOWN
        };
        enum ACCSCALE {
            G2 = 0,
            G4 = 1,
            G8 = 2
        };
        struct State {
            uint16_t x;
            uint16_t y;
            uint16_t z;
        };
        enum ACCEVENT {
            ACC_ORIENTATION,
            ACC_SHAKE
        };   
        typedef void (accEventHandler)(ACCEVENT, void*);
        
       virtual ACCERROR registerEventHandler(accEventHandler*, void*) = 0;
       virtual void unregisterEventHandler() = 0; 
        
       virtual ACCERROR isOnline() = 0;
       virtual ACCERROR getState(State*) = 0;
       virtual ACCERROR setScale(ACCSCALE) = 0;
       virtual ACCERROR getTemperature(int8_t*) = 0;
       
    private:
        
};

#endif /* POCUTERACCELEROMETER_H */

